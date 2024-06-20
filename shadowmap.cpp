//#include "main.h"
//#include "renderer.h"
//
//#define SHADOWMAP_RESO  (1980)
//
//static int m_shadowMapDimension = SHADOWMAP_RESO;
//
//HRESULT InitShadowMap(void)
//{
//    ID3D11Texture2D* m_shadowMap = NULL;
//    ID3D11Device* pD3DDevice = GetD3DDevice();
//    D3D11_TEXTURE2D_DESC shadowMapDesc;
//    ZeroMemory(&shadowMapDesc, sizeof(D3D11_TEXTURE2D_DESC));
//    shadowMapDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
//    shadowMapDesc.MipLevels = 1;
//    shadowMapDesc.ArraySize = 1;
//    shadowMapDesc.SampleDesc.Count = 1;
//    shadowMapDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
//    shadowMapDesc.Height = static_cast<UINT>(m_shadowMapDimension);
//    shadowMapDesc.Width = static_cast<UINT>(m_shadowMapDimension);
//
//    HRESULT hr = pD3DDevice->CreateTexture2D(
//        &shadowMapDesc,
//        nullptr,
//        &m_shadowMap
//    );
//
//    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
//    ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
//    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
//    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
//    depthStencilViewDesc.Texture2D.MipSlice = 0;
//
//    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
//    ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
//    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//    shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
//    shaderResourceViewDesc.Texture2D.MipLevels = 1;
//
//    hr = pD3DDevice->CreateDepthStencilView(
//        m_shadowMap.Get(),
//        &depthStencilViewDesc,
//        &m_shadowDepthView
//    );
//
//    hr = pD3DDevice->CreateShaderResourceView(
//        m_shadowMap.Get(),
//        &shaderResourceViewDesc,
//        &m_shadowResourceView
//    );
//    return hr;
//}