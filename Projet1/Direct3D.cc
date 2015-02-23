#include "Direct3D.hh"

namespace O3D {

	bool Direct3D::init(const char * filename, HWND * hwnd) {
		m_hwnd = hwnd;
		m_file.load(filename);
		HRESULT result = initSwapChain();
		result = initRenderTargetView();
		result = initDepthStencil();
		result = initRasterState();
		result = initViewPort();
		result = initDepthStencilState();

		const Node & node = m_file.getNode("option");
		m_vsync_enabled = std::stoi(node.get_Value("vsync"));


		m_file.clear();
		return true;
	}


	HRESULT Direct3D::initSwapChain() {
		int x, y;
		const Node & node = m_file.getNode("screen");

		x = std::stoi(node.get_Value("width"));
		y = std::stoi(node.get_Value("height"));

		HRESULT result;
		IDXGIFactory * factory;
		IDXGIAdapter * adapter;
		IDXGIOutput * adapterOutput;
		DXGI_MODE_DESC* displayModeList;
		DXGI_ADAPTER_DESC adapterDesc;
		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		unsigned int numModes, numerator, denominator, stringLength, error;

		result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
		if (FAILED(result)) { return result; }

		result = factory->EnumAdapters(0, &adapter);
		if (FAILED(result)) { return result; }

		result = adapter->EnumOutputs(0, &adapterOutput);
		if (FAILED(result)) { return result; }

		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
		if (FAILED(result)) { return result; }

		displayModeList = new DXGI_MODE_DESC[numModes];
		if (!displayModeList) { return E_FAIL; }

		result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
		if (FAILED(result)) { return E_FAIL; }


		for (int i = 0; i < (int)numModes; i++) {
			if (displayModeList[i].Width == (unsigned int)x) {
				if (displayModeList[i].Height == (unsigned int)y) {
					numerator = displayModeList[i].RefreshRate.Numerator;
					denominator = displayModeList[i].RefreshRate.Denominator;
				}
			}
		}

		result = adapter->GetDesc(&adapterDesc);
		if (FAILED(result))	{ return result; }
		m_videoCard_memory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

		error = wcstombs_s(&stringLength, m_videoCard_name, 128, adapterDesc.Description, 128);
		if (error != 0) { return false; }
		delete[] displayModeList;
		displayModeList = 0;

		adapterOutput->Release();
		adapterOutput = 0;
		adapter->Release();
		adapter = 0;

		factory->Release();
		factory = 0;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferDesc.Width = (unsigned int)x;
		swapChainDesc.BufferDesc.Height = (unsigned int)y;

		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

		if (m_vsync_enabled) {
			swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
		}
		else {
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		}

		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = *m_hwnd;

		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 16;

		if (std::stoi(node.get_Value("full"))) {
			swapChainDesc.Windowed = false;
		}
		else {
			swapChainDesc.Windowed = true;
		}
		swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.Flags = 0;

		result = D3D10CreateDeviceAndSwapChain(NULL, D3D10_DRIVER_TYPE_HARDWARE, NULL, 0, D3D10_SDK_VERSION,
			&swapChainDesc, &m_swapChain, &m_device);
		if (FAILED(result))	{ return false; }

		return S_OK;
	}


	HRESULT Direct3D::initRenderTargetView() {
		ID3D10Texture2D* backBufferPtr;

		HRESULT result = m_swapChain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&backBufferPtr);
		if (FAILED(result)) { return E_FAIL; }

		result = m_device->CreateRenderTargetView(backBufferPtr, NULL, &m_renderTargetView);
		if (FAILED(result)) { return E_FAIL; }

		backBufferPtr->Release();
		backBufferPtr = NULL;

		return S_OK;
	}



	HRESULT Direct3D::initDepthStencil() {
		D3D10_TEXTURE2D_DESC depthBufferDesc;
		D3D10_DEPTH_STENCIL_DESC depthStencilDesc;
		D3D10_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		HRESULT result;
		const Node & node = m_file.getNode("screen");
		D3DXVECTOR2 screen = D3DXVECTOR2(std::stoi(node.get_Value("width")), std::stoi(node.get_Value("height")));

		ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
		depthBufferDesc.Width = (uint)screen.x;
		depthBufferDesc.Height = (uint)screen.y;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 64;
		depthBufferDesc.Usage = D3D10_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D10_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;

		result = m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_depthStencilBuffer);
		if (FAILED(result))	{ return E_FAIL; }

		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;

		depthStencilDesc.StencilEnable = true;
		depthStencilDesc.StencilReadMask = 0xFF;
		depthStencilDesc.StencilWriteMask = 0xFF;

		depthStencilDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
		depthStencilDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
		depthStencilDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

		depthStencilDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
		depthStencilDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
		depthStencilDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;


		result = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);
		if (FAILED(result))	{ return E_FAIL; }

		m_device->OMSetDepthStencilState(m_depthStencilState, 1);
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		result = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
		if (FAILED(result))	{ return E_FAIL;	}


		m_device->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
		
		return S_OK;
	}


	HRESULT Direct3D::initRasterState() {
		D3D10_RASTERIZER_DESC rasterDesc;
		HRESULT result;

		rasterDesc.AntialiasedLineEnable = true;
		rasterDesc.CullMode = D3D10_CULL_FRONT;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D10_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = true;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState);
		if (FAILED(result)) { return E_FAIL; }
		m_device->RSSetState(m_rasterState);

		return S_OK;
	}


	HRESULT Direct3D::initViewPort() {
		const Node & node = m_file.getNode("screen");
		m_viewport.Width = std::stoi(node.get_Value("width"));
		m_viewport.Height = std::stoi(node.get_Value("height"));
		m_viewport.MinDepth = 0.0f;
		m_viewport.MaxDepth = 1.0f;
		m_viewport.TopLeftX = 0;
		m_viewport.TopLeftY = 0;

		m_device->RSSetViewports(1, &m_viewport);

		return S_OK;
	}

	HRESULT Direct3D::initDepthStencilState() {
		D3D10_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
		HRESULT result;
		
		ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

		depthDisabledStencilDesc.DepthEnable = false;
		depthDisabledStencilDesc.DepthWriteMask = D3D10_DEPTH_WRITE_MASK_ALL;
		depthDisabledStencilDesc.DepthFunc = D3D10_COMPARISON_LESS;
		depthDisabledStencilDesc.StencilEnable = true;
		depthDisabledStencilDesc.StencilReadMask = 0xFF;
		depthDisabledStencilDesc.StencilWriteMask = 0xFF;
		depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D10_STENCIL_OP_INCR;
		depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.FrontFace.StencilFunc = D3D10_COMPARISON_ALWAYS;
		depthDisabledStencilDesc.BackFace.StencilFailOp = D3D10_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D10_STENCIL_OP_DECR;
		depthDisabledStencilDesc.BackFace.StencilPassOp = D3D10_STENCIL_OP_KEEP;
		depthDisabledStencilDesc.BackFace.StencilFunc = D3D10_COMPARISON_ALWAYS;

		result = m_device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_depthDisabledStencilState);
		return result;
	}


	ID3D10Device * Direct3D::getDevice() {
		return m_device;
	}


	void Direct3D::beginScene(D3DXVECTOR4 color) {
		float col[] = { color.x, color.y, color.z, color.w };
		m_device->ClearRenderTargetView(m_renderTargetView, col);
		m_device->ClearDepthStencilView(m_depthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);
	}


	void Direct3D::endScene() {
		if (m_vsync_enabled) {
			m_swapChain->Present(1, 0);
		}
		else {
			m_swapChain->Present(0, 0);
		}
	}


	void Direct3D::turnZBufferOn() {
		m_device->OMSetDepthStencilState(m_depthStencilState, 1);
	}


	
	void Direct3D::resetOMTarget() {
		m_device->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
		m_device->RSSetViewports(1, &m_viewport);
	}






	void Direct3D::kill() {

		if (m_depthDisabledStencilState) {
			m_depthDisabledStencilState->Release();
			m_depthDisabledStencilState = 0;
		}

		if (m_swapChain) {
			m_swapChain->SetFullscreenState(false, NULL);
		}

		if (m_rasterState) {
			m_rasterState->Release();
			m_rasterState = 0;
		}

		if (m_depthStencilView) {
			m_depthStencilView->Release();
			m_depthStencilView = 0;
		}

		if (m_depthStencilState) {
			m_depthStencilState->Release();
			m_depthStencilState = 0;
		}

		if (m_depthStencilBuffer) {
			m_depthStencilBuffer->Release();
			m_depthStencilBuffer = 0;
		}

		if (m_renderTargetView) {
			m_renderTargetView->Release();
			m_renderTargetView = 0;
		}

		if (m_swapChain) {
			m_swapChain->Release();
			m_swapChain = 0;
		}

		if (m_device) {
			m_device->Release();
			m_device = 0;
		}
	}

	Direct3D::~Direct3D() {
		kill();
	}

}