class CCloseButton :
    public CButton
{
public:
    static HRESULT Create(
        _In_ HWND hwndParent,
        _In_ PCWSTR pWindowName,
        _In_ int pImageFile,
        _In_ SIZE size,
        _In_ POINT pt,
        _Outptr_ CButton** ppButton)
    {
        CCloseButton* pButton = new CCloseButton(pImageFile, size, pt);

        HRESULT hr = pButton->Initialize(hwndParent, pWindowName, pImageFile);
        if (SUCCEEDED(hr))
        {
            ShowWindow(pButton->m_hwnd, SW_SHOW);

            *ppButton = pButton;
            pButton = nullptr;
        }

        delete pButton;

        return hr;
    }

    CCloseButton(
        _In_ int pImageFile, 
        _In_ SIZE size,
        _In_ POINT pt)
        :
        CButton(pImageFile, size, pt)
    {
    }

protected:
    void OnClicked() override;
};

class CQuitAppButton :
    public CButton
{
public:
    static HRESULT Create(
        _In_ HWND hwndParent,
        _In_ PCWSTR pWindowName,
        _In_ int pImageFile,
        _In_ SIZE size,
        _In_ POINT pt,
        _Outptr_ CButton** ppButton)
    {
        CQuitAppButton* pButton = new CQuitAppButton(pImageFile, size, pt);

        HRESULT hr = pButton->Initialize(hwndParent, pWindowName, pImageFile);
        if (SUCCEEDED(hr))
        {
            ShowWindow(pButton->m_hwnd, SW_SHOW);

            *ppButton = pButton;
            pButton = nullptr;
        }

        delete pButton;

        return hr;
    }

    CQuitAppButton(
        _In_ int pImageFile, 
        _In_ SIZE size,
        _In_ POINT pt)
        :
        CButton(pImageFile, size, pt)
    {
    }

protected:
    void OnClicked() override;
};


class CMinimizeButton :
    public CButton
{
public:
    static HRESULT Create(
        _In_ HWND hwndParent,
        _In_ PCWSTR pWindowName,
        _In_ int pImageFile,
        _In_ SIZE size,
        _In_ POINT pt,
        _Outptr_ CButton** ppButton)
    {
        CMinimizeButton* pButton = new CMinimizeButton(pImageFile, size, pt);

        HRESULT hr = pButton->Initialize(hwndParent, pWindowName, pImageFile);
        if (SUCCEEDED(hr))
        {
            ShowWindow(pButton->m_hwnd, SW_SHOW);

            *ppButton = pButton;
            pButton = nullptr;
        }

        delete pButton;

        return hr;
    }

    CMinimizeButton(
        _In_ int pImageFile, 
        _In_ SIZE size,
        _In_ POINT pt)
        :
        CButton(pImageFile, size, pt)
    {
    }

protected:
    void OnClicked() override;
};

class CMaximizeButton :
    public CButton
{
public:
    static HRESULT Create(
        _In_ HWND hwndParent,
        _In_ PCWSTR pWindowName,
        _In_ int pImageFile,
        _In_ SIZE size,
        _In_ POINT pt,
        _Outptr_ CButton** ppButton)
    {
        CMaximizeButton* pButton = new CMaximizeButton(pImageFile, size, pt);

        HRESULT hr = pButton->Initialize(hwndParent, pWindowName, pImageFile);
        if (SUCCEEDED(hr))
        {
            ShowWindow(pButton->m_hwnd, SW_SHOW);

            *ppButton = pButton;
            pButton = nullptr;
        }

        delete pButton;

        return hr;
    }

    CMaximizeButton(
        _In_ int pImageFile, 
        _In_ SIZE size,
        _In_ POINT pt)
        :
        CButton(pImageFile, size, pt)
    {
    }

protected:
    void OnClicked() override;
};

class CDragBox :
    public CButton
{
public:
    static HRESULT Create(
        _In_ HWND hwndParent,
        _In_ PCWSTR pWindowName,
        _In_ int pImageFile,
        _In_ SIZE size,
        _In_ POINT pt,
        _Outptr_ CButton** ppButton)
    {
        CDragBox* pButton = new CDragBox(pImageFile, size, pt);

        HRESULT hr = pButton->Initialize(hwndParent, pWindowName, pImageFile);
        if (SUCCEEDED(hr))
        {
            ShowWindow(pButton->m_hwnd, SW_SHOW);

            *ppButton = pButton;
            pButton = nullptr;
        }

        delete pButton;

        return hr;
    }

    CDragBox(
        _In_ int pImageFile, 
        _In_ SIZE size,
        _In_ POINT pt)
        :
        CButton(pImageFile, size, pt)
    {
    }

protected:
    POINT ptStart = {};
    void OnClicked() override;
    void MouseMove(HWND hwnd, LPARAM lParam) override;
    void LBUTTONDOWN(LPARAM lParam) override;
};
