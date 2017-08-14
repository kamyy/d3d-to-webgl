#pragma once
#ifndef D3D11Demo_h
#define D3D11Demo_h

#include "Camera.h"
#include "Shader.h"
#include "Light.h"
#include "Actor.h"
#include "Timer.h"
#include "ASE.h"

namespace d3d11demo {

    const UINT RENDERING_WINDOW_WD = 1280;
    const UINT RENDERING_WINDOW_HT = 720;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class D3D11DemoCWnd : public CWnd {
        DECLARE_DYNCREATE(D3D11DemoCWnd)
        DECLARE_MESSAGE_MAP()

    public:
        D3D11DemoCWnd();

        BOOL PreCreateWindow(CREATESTRUCT& cs);
        afx_msg BOOL OnEraseBkgnd(CDC* device);
        afx_msg void OnPaint();

        afx_msg void OnLButtonDown(UINT flags, CPoint point);
        afx_msg void OnLButtonUp(UINT flags, CPoint point);
        afx_msg void OnRButtonDown(UINT flags, CPoint point);
        afx_msg void OnRButtonUp(UINT flags, CPoint point);
        afx_msg void OnMButtonDown(UINT flags, CPoint point);
        afx_msg void OnMButtonUp(UINT flags, CPoint point);
        afx_msg void OnMouseMove(UINT flags, CPoint point);

    protected:
        class D3D11DemoCWinApp* m_app; // subclass of CWinApp

        int32_t m_leftX;
        int32_t m_leftY;
        bool    m_leftButtonDown;

        int32_t m_rightX;
        int32_t m_rightY;
        bool    m_rightButtonDown;

        int32_t m_middleX;
        int32_t m_middleY;
        bool    m_middleButtonDown;
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class D3D11DemoCFrameWnd : public CFrameWnd {
        DECLARE_DYNCREATE(D3D11DemoCFrameWnd)
        DECLARE_MESSAGE_MAP()

    public:
        D3D11DemoCFrameWnd();
        CStatusBar m_statusBar;
        D3D11DemoCWnd m_childWnd;

        BOOL PreCreateWindow(CREATESTRUCT& cs);
        afx_msg int OnCreate(LPCREATESTRUCT s);
        afx_msg void OnSysCommand(UINT id, LPARAM lparam);

        afx_msg void OnLButtonDown(UINT flags, CPoint point);
        afx_msg void OnLButtonUp(UINT flags, CPoint point);
        afx_msg void OnRButtonDown(UINT flags, CPoint point);
        afx_msg void OnRButtonUp(UINT flags, CPoint point);
        afx_msg void OnMButtonDown(UINT flags, CPoint point);
        afx_msg void OnMButtonUp(UINT flags, CPoint point);
        afx_msg void OnMouseMove(UINT flags, CPoint point);

        afx_msg void OnFileOpen();
        afx_msg void OnFileExit();

        afx_msg void OnViewCameras(UINT id);
        afx_msg void OnViewLights();
        afx_msg void OnDebugWirefrm();
        afx_msg void OnDebugNormals();
        afx_msg void OnHelpControls();
        afx_msg void OnHelpAbout();

        afx_msg void OnUpdateViewCameras(CCmdUI* ui);
        afx_msg void OnUpdateDebugWirefrm(CCmdUI* ui);
        afx_msg void OnUpdateDebugNormals(CCmdUI* ui);

    protected:
        class D3D11DemoCWinApp* m_app; // subclass of CWinApp
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class D3D11DemoCWinApp : public CWinApp {
        HACCEL      m_haccel;
        Timer       m_timer;

        ID3D11Device*            m_d3dDevice = nullptr;
        IDXGISwapChain*          m_swapChain = nullptr;
        ID3D11DeviceContext*     m_d3dDeviceContext  = nullptr;
        ID3D11RenderTargetView*  m_renderTargetView  = nullptr;
        ID3D11DepthStencilView*  m_depthStencilView  = nullptr;
        ID3D11RasterizerState*   m_rasterizerStateCC = nullptr;
        ID3D11RasterizerState*   m_rasterizerStateCW = nullptr;
        ID3D11DepthStencilState* m_depthStencilStateA = nullptr;
        ID3D11DepthStencilState* m_depthStencilStateB = nullptr;
        ID3D11BlendState*        m_alphaBlendStateA = nullptr;
        ID3D11BlendState*        m_alphaBlendStateB = nullptr;

        shared_ptr<RefFrame>  m_worldSpace = nullptr;
        shared_ptr<RefFrame>  m_activeCamTgt = nullptr;
        shared_ptr<Camera>    m_activeCamera = nullptr;
        shared_ptr<Camera>    m_mirrorCamera = nullptr;
        shared_ptr<AmbientLS> m_ambientLS = nullptr;
        shared_ptr<OmniDirLS> m_omniDirLS = nullptr;

        CameraArray m_cameraArray;
        ActorList*  m_actors = nullptr;
        Actor*      m_mirror = nullptr;

        bool m_drawWirefrm = false;
        bool m_drawNormals = false;

        void resetD3D11Resources();

    public:
        virtual BOOL InitInstance() override;
        virtual int  ExitInstance() override;
        virtual BOOL OnIdle(LONG n) override;

        void loadScene(const ASE& ase);
        void freeScene();
        void render();

        shared_ptr<Camera> getCamera(uint32_t i) const;
        bool               setActiveCamera(uint32_t i);
        shared_ptr<Camera> getActiveCamera() const;

        shared_ptr<AmbientLS> getAmbientLS() const;
        shared_ptr<OmniDirLS> getOmniDirLS() const;

        void toggleDrawWirefrm();
        void toggleDrawNormals();
        void alphaBlendingOn();
        void noAlphaBlending();

        bool isDrawWirefrmOn() const {
            return m_drawWirefrm;
        }
        bool isDrawNormalsOn() const {
            return m_drawNormals;
        }

        uint32_t getCameraCount() const {
            return (uint32_t)m_cameraArray.size();
        }

        ID3D11Device* getD3D11Device() {
            return m_d3dDevice;
        }
        ID3D11DeviceContext* getD3D11DeviceContext() {
            return m_d3dDeviceContext;
        }


    };

    extern D3D11DemoCWinApp g_app;

}

#endif

