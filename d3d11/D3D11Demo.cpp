#include "StdAfx.h"
#include "D3D11Demo.h"
#include "LightsDlg.h"
#include "Resource.h"
#include "DrawableModel.h"
#include "Material.h"
#include "Texture.h"
#include "Shader.h"
#include "ASE.h"

#include "json/single_include/nlohmann/json.hpp"

#include <direct.h>

namespace d3d11demo {

    using json = nlohmann::json;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    IMPLEMENT_DYNCREATE(D3D11DemoCWnd, CWnd)

    BEGIN_MESSAGE_MAP(D3D11DemoCWnd, CWnd)
        ON_WM_ERASEBKGND()
        ON_WM_PAINT()
        ON_WM_LBUTTONDOWN()
        ON_WM_LBUTTONUP()
        ON_WM_RBUTTONDOWN()
        ON_WM_RBUTTONUP()
        ON_WM_MBUTTONDOWN()
        ON_WM_MBUTTONUP()
        ON_WM_MOUSEMOVE()
    END_MESSAGE_MAP()

    D3D11DemoCWnd::D3D11DemoCWnd() {
        m_app = (D3D11DemoCWinApp*)AfxGetApp();
        m_leftX = 0;
        m_leftY = 0;
        m_leftButtonDown = false;
        m_rightX = 0;
        m_rightY = 0;
        m_rightButtonDown = false;
        m_middleX = 0;
        m_middleY = 0;
        m_middleButtonDown = false;
    }

    BOOL D3D11DemoCWnd::PreCreateWindow(CREATESTRUCT& cs) {
        if (!CWnd::PreCreateWindow(cs)) {
            return FALSE;
        }
        return TRUE;
    }

    BOOL D3D11DemoCWnd::OnEraseBkgnd(CDC* dev) {
        return TRUE;
    }

    void D3D11DemoCWnd::OnPaint() {
        CWnd::OnPaint();
        m_app->render();
    }

    void D3D11DemoCWnd::OnLButtonDown(UINT flags, CPoint point) {
        m_leftX = point.x;
        m_leftY = point.y;
        m_leftButtonDown = true;
        if (GetCapture() != this) {
            SetCapture();
        }
    }

    void D3D11DemoCWnd::OnLButtonUp(UINT flags, CPoint point) {
        m_leftButtonDown = false;
        if (!m_leftButtonDown && !m_rightButtonDown && !m_middleButtonDown
                && GetCapture() == this) {
            ReleaseCapture();
        }
    }

    void D3D11DemoCWnd::OnRButtonDown(UINT flags, CPoint point) {
        m_rightX = point.x;
        m_rightY = point.y;
        m_rightButtonDown = true;
        if (GetCapture() != this) {
            SetCapture();
        }
    }

    void D3D11DemoCWnd::OnRButtonUp(UINT flags, CPoint point) {
        m_rightButtonDown = false;
        if (!m_leftButtonDown && !m_rightButtonDown && !m_middleButtonDown
                && GetCapture() == this) {
            ReleaseCapture();
        }
    }

    void D3D11DemoCWnd::OnMButtonDown(UINT flags, CPoint point) {
        m_middleX = point.x;
        m_middleY = point.y;
        m_middleButtonDown = true;
        if (GetCapture() != this) {
            SetCapture();
        }
    }

    void D3D11DemoCWnd::OnMButtonUp(UINT flags, CPoint point) {
        m_middleButtonDown = false;
        if (!m_leftButtonDown && !m_rightButtonDown && !m_middleButtonDown
                && GetCapture() == this) {
            ReleaseCapture();
        }
    }

    void D3D11DemoCWnd::OnMouseMove(UINT flags, CPoint point) {
        auto camera = m_app->getActiveCamera();
        auto target = m_app->getActiveCamera()->getParent();

        if (m_leftButtonDown) {
            int32_t x = point.x;
            int32_t y = point.y;
            if (camera) {
                camera->rotateX(DegressToRadians(float(m_leftY - y)), target.get()); // pitch about camera target's x-axis
                target->rotateZ(DegressToRadians(float(m_leftX - x))); // yaw about camera's own z-axis
            }
            m_leftX = x;
            m_leftY = y;
        }
        if (m_rightButtonDown) {
            int32_t x = point.x;
            int32_t y = point.y;
            if (camera) {
                camera->translate(Vector1x4(0.0f, // move along camera's own y-axis
                                            float(x - m_rightX) * 0.1f,
                                            0.0f,
                                            1.0f));
            }
            m_rightX = x;
            m_rightY = y;
        }
        if (m_middleButtonDown) {
            int32_t x = point.x;
            int32_t y = point.y;
            float dx = float(m_middleX - x) * 0.1f;
            float dz = float(y - m_middleY) * 0.1f;
            if (camera) { // map movement in camera space into target space
                Vector1x4 dir = camera->mapPos(Vector1x4(dx,
                                                         0.f,
                                                         dz,
                                                         0.f),
                                                         target.get());
                target->translate(dir); // move along own axes
            }
            m_middleX = x;
            m_middleY = y;
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    IMPLEMENT_DYNCREATE(D3D11DemoCFrameWnd, CFrameWnd)

    BEGIN_MESSAGE_MAP(D3D11DemoCFrameWnd, CFrameWnd)
        ON_WM_CREATE()
        ON_WM_SYSCOMMAND()

        // D3D9DemoWnd client area mouse activity is redirected to D3D9DemoChildWnd
        ON_WM_LBUTTONDOWN()
        ON_WM_LBUTTONUP()
        ON_WM_RBUTTONDOWN()
        ON_WM_RBUTTONUP()
        ON_WM_MBUTTONDOWN()
        ON_WM_MBUTTONUP()
        ON_WM_MOUSEMOVE()

        ON_COMMAND(ID_FILE_OPENASE, OnFileOpen)
        ON_COMMAND(ID_FILE_SAVEJSON, OnFileSave)
        ON_COMMAND(ID_FILE_EXIT, OnFileExit)

        ON_COMMAND_RANGE(ID_VIEW_CAMERA1, ID_VIEW_CAMERA3, OnViewCameras)
        ON_COMMAND(ID_DEBUG_WIREFRAME, OnDebugWirefrm)
        ON_COMMAND(ID_DEBUG_NORMALS, OnDebugNormals)
        ON_COMMAND(ID_VIEW_LIGHTS, OnViewLights)
        ON_COMMAND(ID_HELP_CONTROLS, OnHelpControls)
        ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)

        ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_CAMERA1, ID_VIEW_CAMERA3, OnUpdateViewCameras)
        ON_UPDATE_COMMAND_UI(ID_DEBUG_WIREFRAME, OnUpdateDebugWirefrm)
        ON_UPDATE_COMMAND_UI(ID_DEBUG_NORMALS, OnUpdateDebugNormals)
    END_MESSAGE_MAP()

    void D3D11DemoCWinApp::resetD3D11Resources() {
        /*
        if (m_pp.Windowed) { // select windowed mode
            // restore saved non-fullscreen window size and position on screen
            m_pMainWnd->MoveWindow( m_rect.left, m_rect.top,
                                    m_rect.right - m_rect.left,
                                    m_rect.bottom - m_rect.top, FALSE);

            m_pp.BackBufferWidth    = 0;
            m_pp.BackBufferHeight   = 0;
            m_pp.BackBufferFormat   = D3DFMT_UNKNOWN;
            m_pp.BackBufferCount    = 1;
            m_pp.hDeviceWindow      = ((D3D9DemoWnd*)m_pMainWnd)->m_childWnd.m_hWnd;

        } else { // select fullscreen exclusive mode
            // retrieve desktop display mode settings (screen size and bit depth)
            D3DDISPLAYMODE mode;
            m_dev->GetDisplayMode(0, &mode);

            m_pp.BackBufferWidth    = mode.Width;
            m_pp.BackBufferHeight   = mode.Height;
            m_pp.BackBufferFormat   = mode.Format;
            m_pp.BackBufferCount    = 1;
            m_pp.hDeviceWindow      = ((D3D9DemoWnd*)m_pMainWnd)->m_hWnd;
        }

        m_rsrcUserMgr->freeD3DResources();
        MY_VALIDATE_HR(m_dev->Reset(&m_pp));
        m_rsrcUserMgr->initD3DResources();
        */
    }


    D3D11DemoCFrameWnd::D3D11DemoCFrameWnd() {
        m_app = (D3D11DemoCWinApp*)AfxGetApp();
    }

    BOOL D3D11DemoCFrameWnd::PreCreateWindow(CREATESTRUCT& cs) {
        if (!CFrameWnd::PreCreateWindow(cs)) {
            return FALSE;
        }

        RECT rect = { 0, 0, RENDERING_WINDOW_WD, RENDERING_WINDOW_HT }; // desired width and height of client area
        AdjustWindowRectEx(&rect, // factor in menu bar, title bar, border etc.
            WS_BORDER | WS_CAPTION | WS_SYSMENU,
            TRUE,
            0);

        cs.style = WS_BORDER | WS_CAPTION | WS_SYSMENU;
        cs.cx = rect.right - rect.left;
        cs.cy = rect.bottom - rect.top;

        return TRUE;
    }

    int D3D11DemoCFrameWnd::OnCreate(LPCREATESTRUCT s) {
        if (CFrameWnd::OnCreate(s) != -1) {
            if (!m_childWnd.CreateEx(0, NULL, NULL, WS_CHILD | WS_VISIBLE,
                CRect(0, 0, RENDERING_WINDOW_WD, RENDERING_WINDOW_HT), this, AFX_IDW_PANE_FIRST)) {
                return -1;
            }

            if (!m_statusBar.Create(this)) {
                return -1;
            }

            UINT indicators[] = {
                ID_SEPARATOR,
                //IDS_AVG_FPS,
                //IDS_AVG_SPF,
                //IDS_CLIENT_SIZE
            };
            m_statusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));
            //m_statusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_NORMAL | SBPS_STRETCH, 0);
            //m_statusBar.SetPaneInfo(1, IDS_AVG_FPS, SBPS_NORMAL, 128);
            //m_statusBar.SetPaneInfo(2, IDS_AVG_SPF, SBPS_NORMAL, 128);
            //m_statusBar.SetPaneInfo(3, IDS_CLIENT_SIZE, SBPS_NORMAL, 128);

            return 0;
        }
        return -1;
    }

    void D3D11DemoCFrameWnd::OnSysCommand(UINT id, LPARAM lparam) {
        CFrameWnd::OnSysCommand(id, lparam);
    }

    void D3D11DemoCFrameWnd::OnLButtonDown(UINT flags, CPoint point) {
        m_childWnd.OnLButtonDown(flags, point);
    }

    void D3D11DemoCFrameWnd::OnLButtonUp(UINT flags, CPoint point) {
        m_childWnd.OnLButtonUp(flags, point);
    }

    void D3D11DemoCFrameWnd::OnRButtonDown(UINT flags, CPoint point) {
        m_childWnd.OnRButtonDown(flags, point);
    }

    void D3D11DemoCFrameWnd::OnRButtonUp(UINT flags, CPoint point) {
        m_childWnd.OnRButtonUp(flags, point);
    }

    void D3D11DemoCFrameWnd::OnMButtonDown(UINT flags, CPoint point) {
        m_childWnd.OnMButtonDown(flags, point);
    }

    void D3D11DemoCFrameWnd::OnMButtonUp(UINT flags, CPoint point) {
        m_childWnd.OnMButtonUp(flags, point);
    }

    void D3D11DemoCFrameWnd::OnMouseMove(UINT flags, CPoint point) {
        m_childWnd.OnMouseMove(flags, point);
    }

    void D3D11DemoCFrameWnd::OnFileOpen() {
        static CString file = "";

        CFileDialog dialog(TRUE, "ASE", file, OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
            "3DS-Max ASCII Export Files (*.ase)|*.ASE||");

        if (dialog.DoModal() == IDOK) {
            Shader::freeSingleton();
            Texture::freeSingleton();
            Material::freeSingleton();

            ASE ase;
            if (!ase.read(dialog.GetPathName())) {
                MessageBox("Error reading 3DS-Max ASE file!", NULL, MB_OK | MB_ICONERROR);
            } else {
                file = dialog.GetPathName();
                m_app->loadScene(ase);
            }
        }
    }

    void D3D11DemoCFrameWnd::OnFileSave() {
        CFileDialog dialog(FALSE, "js", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
            "Scene Export Files (*.json)|*.json||");
        if (dialog.DoModal() == IDOK) {
            m_app->saveScene(dialog.GetPathName());
        }
    }

    void D3D11DemoCFrameWnd::OnFileExit() {
        PostMessage(WM_CLOSE, 0L, 0L);
    }

    void D3D11DemoCFrameWnd::OnViewCameras(UINT id) {
        if (m_app->getCameraCount() > 0) {
            CMenu* menu = GetMenu()->GetSubMenu(1);
            menu->CheckMenuRadioItem(ID_VIEW_CAMERA1, ID_VIEW_CAMERA3, id, MF_BYCOMMAND);
            const uint32_t i = id - ID_VIEW_CAMERA1;
            m_app->setActiveCamera(i);
        }
    }

    void D3D11DemoCFrameWnd::OnViewLights() {
        // LightDlg::PostNcDestroy() will release the memory
        LightsDlg* lightsDlg = new LightsDlg(this);
    }

    void D3D11DemoCFrameWnd::OnDebugWirefrm() {
        m_app->toggleDrawWirefrm();
    }

    void D3D11DemoCFrameWnd::OnDebugNormals() {
        m_app->toggleDrawNormals();
    }

    void D3D11DemoCFrameWnd::OnHelpControls() {
        MessageBox("Left drag to tumble\n"
            "Middle drag to pan\n"
            "Right drag to dolly",
            "Help With Camera Controls", MB_OK | MB_ICONINFORMATION);
    }

    void D3D11DemoCFrameWnd::OnHelpAbout() {
        MessageBox("D3D11 Demo by Kam Y Yip\n"
            "kyip@ymail.com\n\n",
            "About D3D11 Demo", MB_OK | MB_ICONINFORMATION);
    }

    void D3D11DemoCFrameWnd::OnUpdateViewCameras(CCmdUI* ui) {
        uint32_t cameraCount = m_app->getCameraCount();

        ui->Enable(TRUE);
        ui->SetRadio(FALSE);

        switch (ui->m_nID) {
        case ID_VIEW_CAMERA1:
            if (cameraCount > 0) {
                ui->SetRadio(m_app->getActiveCamera() == m_app->getCamera(0));
            } else {
                ui->Enable(FALSE);
            }
            break;
        case ID_VIEW_CAMERA2:
            if (cameraCount > 1) {
                ui->SetRadio(m_app->getActiveCamera() == m_app->getCamera(1));
            } else {
                ui->Enable(FALSE);
            }
            break;
        case ID_VIEW_CAMERA3:
            if (cameraCount > 2) {
                ui->SetRadio(m_app->getActiveCamera() == m_app->getCamera(2));
            } else {
                ui->Enable(FALSE);
            }
            break;
        }
    }

    void D3D11DemoCFrameWnd::OnUpdateDebugWirefrm(CCmdUI* ui) {
        if (m_app->isDrawWirefrmOn()) {
            ui->SetCheck(1);
        } else {
            ui->SetCheck(0);
        }
    }

    void D3D11DemoCFrameWnd::OnUpdateDebugNormals(CCmdUI* ui) {
        if (m_app->isDrawNormalsOn()) {
            ui->SetCheck(1);
        } else {
            ui->SetCheck(0);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    D3D11DemoCWinApp g_app;

    BOOL D3D11DemoCWinApp::InitInstance() {
        InitCommonControls();

        D3D11DemoCFrameWnd* frameWnd = new D3D11DemoCFrameWnd;
        m_pMainWnd = frameWnd;

        frameWnd->LoadFrame(IDR_AVATAR3D, 0);

        if (frameWnd->m_hWnd && frameWnd->m_childWnd.m_hWnd) {

            #ifdef _DEBUG
            UINT creationFlags = D3D11_CREATE_DEVICE_DEBUG;
            #else
            UINT creationFlags = 0;
            #endif

            D3D_FEATURE_LEVEL featureLevels[] = {
                D3D_FEATURE_LEVEL_9_1
            };

            DXGI_SWAP_CHAIN_DESC swapChainDesc;
            ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
            swapChainDesc.BufferDesc.Width                   = RENDERING_WINDOW_WD;
            swapChainDesc.BufferDesc.Height                  = RENDERING_WINDOW_HT;
            swapChainDesc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
            swapChainDesc.BufferDesc.RefreshRate.Numerator   = 0;
            swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
            swapChainDesc.SampleDesc.Count                   = 1; // MSAA
            swapChainDesc.SampleDesc.Quality                 = 0; // Quality
            swapChainDesc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
            swapChainDesc.BufferCount                        = 2;
            swapChainDesc.OutputWindow                       = frameWnd->m_childWnd.m_hWnd;
            swapChainDesc.Windowed                           = TRUE;
            swapChainDesc.SwapEffect                         = DXGI_SWAP_EFFECT_DISCARD;

            ThrowIfFailed(
                D3D11CreateDeviceAndSwapChain(
                    nullptr,
                    D3D_DRIVER_TYPE_HARDWARE,
                    nullptr,
                    creationFlags,
                    featureLevels,
                    ARRAYSIZE(featureLevels),
                    D3D11_SDK_VERSION,
                    &swapChainDesc,
                    &m_swapChain,
                    &m_d3dDevice,
                    nullptr,
                    &m_d3dDeviceContext
                )
            );

            ID3D11Texture2D* backBuffer;
            ID3D11Texture2D* depthStencilBuffer;

            ThrowIfFailed(
                m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer)
            );
            ThrowIfFailed(
                m_d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView)
            );
            ThrowIfFailed(
                m_d3dDevice->CreateTexture2D(
                    &CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_D24_UNORM_S8_UINT,
                                           RENDERING_WINDOW_WD,
                                           RENDERING_WINDOW_HT,
                                           1,
                                           1,
                                           D3D11_BIND_DEPTH_STENCIL),
                    nullptr,
                    &depthStencilBuffer
                )
            );
            ThrowIfFailed(
                m_d3dDevice->CreateDepthStencilView(depthStencilBuffer, nullptr, &m_depthStencilView)
            );

            {   CD3D11_RASTERIZER_DESC desc(CD3D11_DEFAULT{}); // back face cull CW
                desc.FillMode              = m_drawWirefrm ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
                desc.FrontCounterClockwise = TRUE;
                ThrowIfFailed(
                    m_d3dDevice->CreateRasterizerState(&desc, &m_rasterizerStateCC)
                );
            }
            {   CD3D11_RASTERIZER_DESC desc(CD3D11_DEFAULT{}); // back face cull CC
                desc.FillMode              = m_drawWirefrm ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
                desc.FrontCounterClockwise = FALSE;
                ThrowIfFailed(
                    m_d3dDevice->CreateRasterizerState(&desc, &m_rasterizerStateCW)
                );
            }
            {   CD3D11_DEPTH_STENCIL_DESC desc(CD3D11_DEFAULT{}); // use to create stencil mask for mirror shape
                desc.DepthWriteMask          = D3D11_DEPTH_WRITE_MASK_ZERO; // disable depth write
                desc.StencilEnable           = true;                        // enable stencil test
                desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;    // set stencil to 1 when both depth and stencil test pass
                desc.FrontFace.StencilFunc   = D3D11_COMPARISON_ALWAYS;     // stencil test always passes
                ThrowIfFailed(
                    m_d3dDevice->CreateDepthStencilState(&desc, &m_depthStencilStateA)
                );
            }
            {   CD3D11_DEPTH_STENCIL_DESC desc(CD3D11_DEFAULT{}); // use to render reflection using stencil mask into back buffer
                desc.DepthWriteMask          = D3D11_DEPTH_WRITE_MASK_ALL; // enable depth write
                desc.StencilEnable           = true;                       // enable stencil test
                desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;      // don't change stencil when both depth and stencil test pass
                desc.FrontFace.StencilFunc   = D3D11_COMPARISON_EQUAL;     // stencil test passes when stencil equals 1
                ThrowIfFailed(
                    m_d3dDevice->CreateDepthStencilState(&desc, &m_depthStencilStateB)
                );
            }
            {   CD3D11_BLEND_DESC desc(CD3D11_DEFAULT{}); // prevent changes to back buffer
                desc.RenderTarget[0].BlendEnable           = true;
                desc.RenderTarget[0].SrcBlend              = D3D11_BLEND_ZERO;
                desc.RenderTarget[0].DestBlend             = D3D11_BLEND_ONE;
                ThrowIfFailed(
                    m_d3dDevice->CreateBlendState(&desc, &m_alphaBlendStateA)
                );
            }
            {   CD3D11_BLEND_DESC desc(CD3D11_DEFAULT{}); // source alpha blend pixel shader output with back buffer
                desc.RenderTarget[0].BlendEnable           = true;
                desc.RenderTarget[0].SrcBlend              = D3D11_BLEND_SRC_ALPHA;
                desc.RenderTarget[0].DestBlend             = D3D11_BLEND_INV_SRC_ALPHA;
                ThrowIfFailed(
                    m_d3dDevice->CreateBlendState(&desc, &m_alphaBlendStateB)
                );
            }

            m_d3dDeviceContext->RSSetState(m_rasterizerStateCC);
            m_d3dDeviceContext->RSSetViewports(
                1,
                &CD3D11_VIEWPORT( 0.0f,
                                  0.0f,
                                  static_cast<float>(RENDERING_WINDOW_WD),
                                  static_cast<float>(RENDERING_WINDOW_HT) )
            );

            m_d3dDeviceContext->OMSetRenderTargets(
                1,
                &m_renderTargetView,
                m_depthStencilView
            );

            depthStencilBuffer->Release();
            backBuffer->Release();

            Material::initSingleton();
            Texture::initSingleton();
            Shader::initSingleton();

            ASE ase;
            _chdir("scene");
            if (ase.read("avatar3d.ase")) {
                loadScene(ase);
            }

            frameWnd->ShowWindow(SW_SHOW);
            frameWnd->UpdateWindow();
            return TRUE;
        }

        return FALSE;
    }

    int D3D11DemoCWinApp::ExitInstance() {
        freeScene();

        Material::freeSingleton();
        Texture::freeSingleton();
        Shader::freeSingleton();

        if (m_d3dDevice)
            m_d3dDevice->Release();
        if (m_swapChain)
            m_swapChain->Release();
        if (m_d3dDeviceContext)
            m_d3dDeviceContext->Release();
        if (m_renderTargetView)
            m_renderTargetView->Release();
        if (m_rasterizerStateCC)
            m_rasterizerStateCC->Release();
        if (m_rasterizerStateCW)
            m_rasterizerStateCW->Release();
        if (m_depthStencilView)
            m_depthStencilView->Release();
        if (m_depthStencilStateA)
            m_depthStencilStateA->Release();
        if (m_depthStencilStateB)
            m_depthStencilStateB->Release();
        if (m_alphaBlendStateA)
            m_alphaBlendStateA->Release();
        if (m_alphaBlendStateB)
            m_alphaBlendStateB->Release();

        return CWinApp::ExitInstance();
    }

    BOOL D3D11DemoCWinApp::OnIdle(LONG n) {
        if (!CWinApp::OnIdle(n)) {
            render();
        }
        return TRUE;
    }

    void D3D11DemoCWinApp::loadScene(const ASE& ase) {
        freeScene();

        m_worldSpace = make_shared<RefFrame>();

        ase.createCameras(m_cameraArray, m_worldSpace);
        if (m_cameraArray.empty()) { // no cameras in ase so create a default along with something to target
            auto tgt = make_shared<RefFrame>(m_worldSpace);
            auto cam = make_shared<Camera>( CAMERA_DEFAULT_FOV,
                                            float(RENDERING_WINDOW_WD) / (RENDERING_WINDOW_HT),
                                            CAMERA_DEFAULT_N_CLIP_DISTANCE,
                                            CAMERA_DEFAULT_F_CLIP_DISTANCE, tgt);
            m_cameraArray.push_back(cam);
            cam->translate(Vector1x4(0.0f, -12.0f, 1.0f, 1.0f));
        }
        m_activeCamera = m_cameraArray.at(0);

        m_actors = new ActorList;
        ase.createActors(m_actors, m_worldSpace);

        for (auto i = m_actors->begin(); i != m_actors->end(); ++i) {
            if (!strcmp((*i)->getName(), "Avatar3DMirror")) {
                m_mirror = *i;
                m_actors->erase(i);
                m_mirrorCamera = make_shared<Camera>(CAMERA_DEFAULT_FOV,
                                                     float(RENDERING_WINDOW_WD) / (RENDERING_WINDOW_HT),
                                                     CAMERA_DEFAULT_N_CLIP_DISTANCE,
                                                     CAMERA_DEFAULT_F_CLIP_DISTANCE, m_worldSpace);
                break;
            }
        }

        m_ambientLS = make_shared<AmbientLS>(LightsDlg::getLowerHemisphereRGB(),
                                             LightsDlg::getUpperHemisphereRGB());

        m_omniDirLS = make_shared<OmniDirLS>(LightsDlg::getOmniDirectionalRGB(),
                                             LightsDlg::getAttenCoeff0(),
                                             LightsDlg::getAttenCoeff1(),
                                             LightsDlg::getAttenCoeff2(),
                                             m_activeCamera);
    }

    void D3D11DemoCWinApp::saveScene(const char* filename) {
        json obj = {
            { "sceneRoot", m_worldSpace->toJSON() },
            { "materials", Material::getMap() },
            { "textures", Texture::getMap() }
        };
        ofstream of(filename);
        of << setw(4) << obj;
    }

    void D3D11DemoCWinApp::freeScene() {
        m_worldSpace   = nullptr;
        m_activeCamera = nullptr;
        m_mirrorCamera = nullptr;
        m_cameraArray.clear();
        m_ambientLS = nullptr;
        m_omniDirLS = nullptr;

        delete m_actors;
        m_actors = nullptr;
        delete m_mirror;
        m_mirror = nullptr;
    }

    void D3D11DemoCWinApp::render() {
        VS_REG_B0 vs;
        PS_REG_B0 ps;

        vs.g_att_coeff[0] = g_app.getOmniDirLS()->m_coeff0;
        vs.g_att_coeff[1] = g_app.getOmniDirLS()->m_coeff1;
        vs.g_att_coeff[2] = g_app.getOmniDirLS()->m_coeff2;
        vs.g_att_coeff[3] = 0.0f;
        Shader::setVS_REG_B0(vs);

        ps.g_intensity = g_app.getOmniDirLS()->m_color;
        ps.g_gnd = g_app.getAmbientLS()->m_lowerHemisphereColor;
        ps.g_sky = g_app.getAmbientLS()->m_upperHemisphereColor;
        Shader::setPS_REG_B0(ps);

        m_d3dDeviceContext->ClearRenderTargetView(m_renderTargetView, DirectX::Colors::CornflowerBlue);
        m_d3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

        if (m_mirror) {
            m_d3dDeviceContext->OMSetDepthStencilState(m_depthStencilStateA, 1); // write 1 to stencil if depth test passes, enable depth test, disable depth write
            m_d3dDeviceContext->OMSetBlendState(m_alphaBlendStateA, nullptr, 0xffffffff); // prevent changes to back buffer
            m_mirror->draw(true); // draw mirror to create the stencil mask in shape of mirror

            m_d3dDeviceContext->RSSetState(m_rasterizerStateCW);                 // cull CC
            m_d3dDeviceContext->OMSetDepthStencilState(m_depthStencilStateB, 1); // draw only where stencil is set to 1, enable depth test/write
            m_d3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);   // no alpha blending

            Matrix4x4 m = m_activeCamera->getModelMatrix(); // setup mirror camera
            m *= !m_mirror->getModel(0)->getModelMatrix();
            m.postCatSxyz(1.0f, 1.0f, -1.0f);
            m *=  m_mirror->getModel(0)->getModelMatrix();
            m_mirrorCamera->setModelMatrix(m);

            auto oldCamera = m_activeCamera;
            m_activeCamera = m_mirrorCamera;
            m_actors->draw(true); // draw reflection
            m_activeCamera = oldCamera;

            m_d3dDeviceContext->RSSetState(m_rasterizerStateCC);    // cull CW
            m_d3dDeviceContext->OMSetDepthStencilState(nullptr, 1); // disable stencil, enable depth test/write
            m_d3dDeviceContext->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH,  1.0f, 0); // reset depth buffer
        }

        if (m_actors && m_drawNormals) {
            m_actors->drawVertexNormals();
        }
        if (m_mirror) {
            m_mirror->draw(false);
        }
        if (m_actors) {
            m_actors->draw(false);
        }
        DrawBatchBase::flushAlphaQ();

        ThrowIfFailed(
            m_swapChain->Present(0, 0)
        );
    }

    shared_ptr<Camera> D3D11DemoCWinApp::getCamera(uint32_t i) const {
        if (i < m_cameraArray.size()) {
            return m_cameraArray[i];
        }
        return nullptr;
    }

    bool D3D11DemoCWinApp::setActiveCamera(uint32_t i) {
        if (i < m_cameraArray.size()) {
            m_activeCamera = m_cameraArray[i];
            return true;
        }
        return false;
    }

    shared_ptr<Camera> D3D11DemoCWinApp::getActiveCamera() const {
        return m_activeCamera;
    }

    shared_ptr<AmbientLS> D3D11DemoCWinApp::getAmbientLS() const {
        return m_ambientLS;
    }

    shared_ptr<OmniDirLS> D3D11DemoCWinApp::getOmniDirLS() const {
        return m_omniDirLS;
    }


    void D3D11DemoCWinApp::toggleDrawWirefrm() {
        m_drawWirefrm = !m_drawWirefrm;
        {   CD3D11_RASTERIZER_DESC desc(CD3D11_DEFAULT{});
            desc.FillMode              = m_drawWirefrm ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
            desc.FrontCounterClockwise = TRUE;

            if (m_rasterizerStateCC) {
                m_rasterizerStateCC->Release();
                m_rasterizerStateCC = nullptr;
            }
            ThrowIfFailed(
                m_d3dDevice->CreateRasterizerState(&desc, &m_rasterizerStateCC)
            );
        }
        {   CD3D11_RASTERIZER_DESC desc(CD3D11_DEFAULT{});
            desc.FillMode              = m_drawWirefrm ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
            desc.FrontCounterClockwise = FALSE;

            if (m_rasterizerStateCW) {
                m_rasterizerStateCW->Release();
                m_rasterizerStateCW = nullptr;
            }
            ThrowIfFailed(
                m_d3dDevice->CreateRasterizerState(&desc, &m_rasterizerStateCW)
            );
        }

        m_d3dDeviceContext->RSSetState(m_rasterizerStateCC);
    }

    void D3D11DemoCWinApp::toggleDrawNormals() {
        m_drawNormals = !m_drawNormals;
    }

    void D3D11DemoCWinApp::alphaBlendingOn() {
        m_d3dDeviceContext->OMSetBlendState(m_alphaBlendStateB, nullptr, 0xffffffff);
    }

    void D3D11DemoCWinApp::noAlphaBlending() {
        m_d3dDeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
    }

}

