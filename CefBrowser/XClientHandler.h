#pragma once

class XClientHandler : public CefClient,
    public CefContextMenuHandler,
    public CefDisplayHandler,
    public CefGeolocationHandler,
    public CefKeyboardHandler,
    public CefLifeSpanHandler,
    public CefLoadHandler,
    public CefRenderHandler,
    public CefRequestHandler
{
public:
    XClientHandler(void);
    ~XClientHandler(void);

    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE
    {
        return this;
    }
    virtual CefRefPtr<CefRenderHandler> GetRenderHandler() OVERRIDE
    {
        return this;
    }
    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() OVERRIDE
    {
        return this;
    }

    // CefLifeSpanHandler methods
    virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        const CefString& target_url,
        const CefString& target_frame_name,
        const CefPopupFeatures& popupFeatures,
        CefWindowInfo& windowInfo,
        CefRefPtr<CefClient>& client,
        CefBrowserSettings& settings,
        bool* no_javascript_access) OVERRIDE;
    virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual bool DoClose(CefRefPtr<CefBrowser> browser) OVERRIDE;
    virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) OVERRIDE;

    // CefRenderHandler methods
    bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect);
    virtual void OnPopupShow(CefRefPtr<CefBrowser> browser, bool show) OVERRIDE;
    virtual void OnPopupSize(CefRefPtr<CefBrowser> browser,
        const CefRect& rect) OVERRIDE;
    virtual void OnPaint(CefRefPtr<CefBrowser> browser,
        PaintElementType type,
        const RectList& dirtyRects,
        const void* buffer,
        int width,
        int height) OVERRIDE;

    // CefDisplayHandler methods
    virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
        CefRefPtr<CefFrame> frame,
        const CefString& url) OVERRIDE;
    virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
        const CefString& title) OVERRIDE;

    // Include the default reference counting implementation.
    IMPLEMENT_REFCOUNTING(XClientHandler);
    // Include the default locking implementation.
    IMPLEMENT_LOCKING(XClientHandler);
};
