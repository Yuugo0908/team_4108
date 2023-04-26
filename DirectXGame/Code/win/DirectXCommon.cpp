#include "DirectXCommon.h"
#include <vector>
#include <cassert>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#include "SafeDelete.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

using namespace Microsoft::WRL;

DirectXCommon* DirectXCommon::GetInstance()
{
	static DirectXCommon instance;

	return &instance;
}

void DirectXCommon::Initialize(WinApp* winApp)
{
	// nullptr�`�F�b�N
	assert(winApp);

	this->winApp = winApp;

	// FPS�Œ菉����
	InitializeFixFPS();

	// DXGI�f�o�C�X������
	if (!InitializeDXGIDevice())
	{
		assert(0);
	}

	// �R�}���h�֘A������
	if (!InitializeCommand())
	{
		assert(0);
	}

	// �X���b�v�`�F�[���̐���
	if (!CreateSwapChain())
	{
		assert(0);
	}

	// �����_�[�^�[�Q�b�g����
	if (!CreateFinalRenderTargets())
	{
		assert(0);
	}

	// �[�x�o�b�t�@����
	if (!CreateDepthBuffer())
	{
		assert(0);
	}

	// �t�F���X����
	if (!CreateFence())
	{
		assert(0);
	}

	// imgui������
	if (!InitImgui())
	{
		assert(0);
	}
}

void DirectXCommon::PreDraw()
{
	// �o�b�N�o�b�t�@�̔ԍ����擾�i2�Ȃ̂�0�Ԃ�1�ԁj
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
	// ���\�[�X�o���A��ύX�i�\����ԁ��`��Ώہj
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), bbIndex, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// �����_�[�^�[�Q�b�g���Z�b�g
	commandList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	// �S��ʃN���A
	ClearRenderTarget();
	// �[�x�o�b�t�@�N���A
	ClearDepthBuffer();

	// �r���[�|�[�g�̐ݒ�
	commandList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, WinApp::window_width, WinApp::window_height));
	// �V�U�����O��`�̐ݒ�
	commandList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, WinApp::window_width, WinApp::window_height));

	// imgui�J�n
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void DirectXCommon::PostDraw()
{
	// imgui�`��
	ImGui::Render();
	ID3D12DescriptorHeap* ppHeaps[] = { imguiHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());

	// ���\�[�X�o���A��ύX�i�`��Ώہ��\����ԁj
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();
	commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// ���߂̃N���[�Y
	commandList->Close();

	// �R�}���h���X�g�̎��s
	ID3D12CommandList* cmdLists[] = { commandList.Get() }; // �R�}���h���X�g�̔z��
	commandQueue->ExecuteCommandLists(1, cmdLists);

	// �o�b�t�@���t���b�v
	swapchain->Present(1, 0);

	// �R�}���h���X�g�̎��s������҂�
	commandQueue->Signal(fence.Get(), ++fenceVal);
	if (fence->GetCompletedValue() != fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		fence->SetEventOnCompletion(fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}

	// FPS�Œ�X�V
	UpdateFixFPS();

	commandAllocator->Reset(); // �L���[���N���A
	commandList->Reset(commandAllocator.Get(), nullptr); // �ĂуR�}���h���X�g�𒙂߂鏀��
}

void DirectXCommon::ClearRenderTarget()
{
	UINT bbIndex = swapchain->GetCurrentBackBufferIndex();

	// �����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), bbIndex, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));

	// �S��ʃN���A        Red   Green Blue  Alpha
	float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	commandList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
}

void DirectXCommon::ClearDepthBuffer()
{
	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// �[�x�o�b�t�@�̃N���A
	commandList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

bool DirectXCommon::InitializeDXGIDevice()
{
	HRESULT result = S_FALSE;

#ifdef _DEBUG
	ComPtr<ID3D12Debug1> debugController;
	//�f�o�b�O���C���[���I����
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		// ��{�͗L�������������悢���A���삪�d���Ȃ�̂Œ��ӁA�������������ꍇ�̓R�����g�A�E�g��
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif

	// �Ή����x���̔z��
	D3D_FEATURE_LEVEL levels[] =
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	// DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// �A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter1>> adapters;
	// �����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter1> tmpAdapter;
	for (int i = 0;
		dxgiFactory->EnumAdapters1(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter); // ���I�z��ɒǉ�����
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc); // �A�_�v�^�[�̏����擾

		// �\�t�g�E�F�A�f�o�C�X�����
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}

		std::wstring strDesc = adesc.Description; // �A�_�v�^�[��

		// Intel UHD Graphics�i�I���{�[�h�O���t�B�b�N�j�����
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i]; // �̗p
			break;
		}
	}

	D3D_FEATURE_LEVEL featureLevel;

	result = S_FALSE;
	for (int i = 0; i < _countof(levels); i++)
	{
		// �f�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device));
		if (SUCCEEDED(result))
		{
			// �f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

#ifdef _DEBUG
	/*ComPtr<ID3D12InfoQueue> infoQueue;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&infoQueue))))
	{
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
	}*/
#endif

	return true;
}

bool DirectXCommon::CreateSwapChain()
{
	HRESULT result = S_FALSE;

	// �e��ݒ�����ăX���b�v�`�F�[���𐶐�
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	swapchainDesc.Width = WinApp::window_width;
	swapchainDesc.Height = WinApp::window_height;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F���̏�������ʓI�Ȃ��̂�
	swapchainDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�Ƃ��Ďg����悤��
	swapchainDesc.BufferCount = 2; // �o�b�t�@�����Q�ɐݒ�
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͑��₩�ɔj��
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // �t���X�N���[���؂�ւ�������
	//IDXGISwapChain1��ComPtr��p��
	ComPtr<IDXGISwapChain1> swapchain1;
	HWND hwnd = winApp->GetHwnd();
	//�X���b�v�`�F�[���𐶐�
	result = dxgiFactory->CreateSwapChainForHwnd
	(
		commandQueue.Get(),
		winApp->GetHwnd(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1
	);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}
	swapchain1.As(&swapchain);

	return true;
}

bool DirectXCommon::InitializeCommand()
{
	HRESULT result = S_FALSE;

	// �R�}���h�A���P�[�^�𐶐�
	result = device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// �R�}���h���X�g�𐶐�
	result = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// �W���ݒ�ŃR�}���h�L���[�𐶐�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	result = device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&commandQueue));
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	return true;
}

bool DirectXCommon::CreateFinalRenderTargets()
{
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// �e��ݒ�����ăf�B�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	heapDesc.NumDescriptors = swcDesc.BufferCount;
	result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// ���\�̂Q���ɂ���
	backBuffers.resize(swcDesc.BufferCount);
	for (int i = 0; i < backBuffers.size(); i++)
	{
		// �X���b�v�`�F�[������o�b�t�@���擾
		result = swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		if (FAILED(result))
		{
			assert(0);
			return false;
		}

		// �f�B�X�N���v�^�q�[�v�̃n���h�����擾
		CD3DX12_CPU_DESCRIPTOR_HANDLE handle = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeaps->GetCPUDescriptorHandleForHeapStart(), i, device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV));
		// �����_�[�^�[�Q�b�g�r���[�̐���
		device->CreateRenderTargetView
		(
			backBuffers[i].Get(),
			nullptr,
			handle
		);
	}

	return true;
}

bool DirectXCommon::CreateDepthBuffer()
{
	HRESULT result = S_FALSE;

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D
	(
		DXGI_FORMAT_D32_FLOAT,
		WinApp::window_width,
		WinApp::window_height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
	);
	// ���\�[�X�̐���
	result = device->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE, // �[�x�l�������݂Ɏg�p
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&depthBuffer)
	);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// �[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1; // �[�x�r���[��1��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV; // �f�v�X�X�e���V���r���[
	result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// �[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT; // �[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device->CreateDepthStencilView
	(
		depthBuffer.Get(),
		&dsvDesc,
		dsvHeap->GetCPUDescriptorHandleForHeapStart()
	);

	return true;
}

bool DirectXCommon::CreateFence()
{
	HRESULT result = S_FALSE;

	// �t�F���X�̐���
	result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	return true;
}

bool DirectXCommon::InitImgui()
{
	HRESULT result = S_FALSE;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	result = device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&imguiHeap));
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	// �X���b�v�`�F�[���̏����擾
	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = swapchain->GetDesc(&swcDesc);
	if (FAILED(result))
	{
		assert(0);
		return false;
	}

	if (ImGui::CreateContext() == nullptr)
	{
		assert(0);
		return false;
	}

	if (!ImGui_ImplWin32_Init(winApp->GetHwnd()))
	{
		assert(0);
		return false;
	}

	if (!ImGui_ImplDX12_Init(
		GetDevice(),
		swcDesc.BufferCount,
		swcDesc.BufferDesc.Format,
		imguiHeap.Get(),
		imguiHeap->GetCPUDescriptorHandleForHeapStart(),
		imguiHeap->GetGPUDescriptorHandleForHeapStart()
	))
	{
		assert(0);
		return false;
	}

	return true;
}

void DirectXCommon::InitializeFixFPS()
{
	// ���ݎ��Ԃ��L�^����
	reference = std::chrono::steady_clock::now();
}

void DirectXCommon::UpdateFixFPS()
{
	// 1/60�b�҂�����̎���
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60�b���킸���ɒZ������
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));
	// ���݂̎��Ԃ��擾����
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	// �O��̋L�^����̌o�ߎ��Ԃ��擾����
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference);

	// 1/60(���킸���ɒZ������)�o���Ă��Ȃ��ꍇ
	if (elapsed < kMinCheckTime)
	{
		// 1/60�b�o�߂���܂Ŕ����ȃX���[�v���J��Ԃ�
		while (std::chrono::steady_clock::now() - reference < kMinTime)
		{
			// 1�}�C�N���b�X���[�v
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}

	// ���݂̎��Ԃ��L�^����
	reference = std::chrono::steady_clock::now();
}
