#ifndef SFRENDER_H__
#define SFRENDER_H__

#include "SFCommon.h"
#include <d3d9.h>

#ifndef __D3DX9CORE_H__
struct ID3DXSprite;

typedef enum _D3DXIMAGE_FILEFORMAT {
	D3DXIFF_BMP = 0,
	D3DXIFF_JPG = 1,
	D3DXIFF_TGA = 2,
	D3DXIFF_PNG = 3,
	D3DXIFF_DDS = 4,
	D3DXIFF_PPM = 5,
	D3DXIFF_DIB = 6,
	D3DXIFF_HDR = 7, // high dynamic range formats
	D3DXIFF_PFM = 8, //
	D3DXIFF_FORCE_DWORD = 0x7fffffff
} D3DXIMAGE_FILEFORMAT;

typedef struct _D3DXIMAGE_INFO {
	UINT Width;
	UINT Height;
	UINT Depth;
	UINT MipLevels;
	D3DFORMAT Format;
	D3DRESOURCETYPE ResourceType;
	D3DXIMAGE_FILEFORMAT ImageFileFormat;
} D3DXIMAGE_INFO;
#endif

enum FontFlag
{
	FCR_NONE								= 0x0,
	FCR_BOLD								= 0x1,
	FCR_ITALICS								= 0x2,
	FCR_BORDER								= 0x4,
	FCR_SHADOW								= 0x8,
	FCR_UNDERLINE							= 0x10,
	FCR_STRIKEOUT							= 0x20,
};

enum eTextureTypes
{
	TEXTURE_FROM_FILE,
	TEXTURE_FROM_RESOURCE,
	TEXTURE_FROM_MEMORY
};

struct stTextureInfo
{
	IDirect3DTexture9						*pTexture;
	ID3DXSprite								*pSprite;
	D3DXIMAGE_INFO							imgInfo;

	// from file
	char									szTexturePath[MAX_PATH];
	// from resource
	int										iResourceID;
	D3DCOLOR								D3DColorKey;
	// from memory
	LPCVOID pSrcData;
	UINT SrcDataSize;

	eTextureTypes							eTextureType;
};

template <typename T>
struct rect
{
	T _left;
	T _top;
	T _right;
	T _bottom;

	rect(T left, T top, T right, T bottom) : _left(left), _top(top), _right(right), _bottom(bottom) {}
	rect(T v) : _left(v), _top(v), _right(v), _bottom(v) {}
	rect(void) : rect(0, 0, 0, 0) {}
};
typedef rect<float>							frect;

struct SAMPFUNCS_API stFontInfo
{
	class CD3DFont							*pFont;
	char									m_szFontName[32];
	int										m_fontHeight;
	DWORD									m_dwCreateFlags;
	float									m_fChrHeight;

	HRESULT									Print(const char *text, D3DCOLOR color, float x, float y,
												  bool skipColorTags = false, bool ignoreColorTags = false,
												  frect rect = frect(0.0f));
	HRESULT									PrintShadow(const char *text, D3DCOLOR color, float x, float y,
														bool skipColorTags = false, bool ignoreColorTags = false,
														frect rect = frect(0.0f));

	float									DrawLength(const char *text, bool noColorFormat = false) const;
	size_t									GetCharPos(const char *text, float x, bool noColorFormat = false) const;
	float									DrawHeight(void) const;
};

enum eDirect3DDeviceMethods
{
	D3DMETHOD_QUERYINTERFACE,
	D3DMETHOD_ADDREF,
	D3DMETHOD_RELEASE,
	D3DMETHOD_TESTCOOPERATIVELEVEL,
	D3DMETHOD_GETAVAILABLETEXTUREMEM,
	D3DMETHOD_EVICTMANAGEDRESOURCES,
	D3DMETHOD_GETDIRECT3D,
	D3DMETHOD_GETDEVICECAPS,
	D3DMETHOD_GETDISPLAYMODE,
	D3DMETHOD_GETCREATIONPARAMETERS,
	D3DMETHOD_SETCURSORPROPERTIES,
	D3DMETHOD_SETCURSORPOSITION,
	D3DMETHOD_SHOWCURSOR,
	D3DMETHOD_CREATEADDITIONALSWAPCHAIN,
	D3DMETHOD_GETSWAPCHAIN,
	D3DMETHOD_GETNUMBEROFSWAPCHAINS,
	D3DMETHOD_RESET,
	D3DMETHOD_PRESENT,
	D3DMETHOD_GETBACKBUFFER,
	D3DMETHOD_GETRASTERSTATUS,
	D3DMETHOD_SETDIALOGBOXMODE,
	D3DMETHOD_SETGAMMARAMP,
	D3DMETHOD_GETGAMMARAMP,
	D3DMETHOD_CREATETEXTURE,
	D3DMETHOD_CREATEVOLUMETEXTURE,
	D3DMETHOD_CREATECUBETEXTURE,
	D3DMETHOD_CREATEVERTEXBUFFER,
	D3DMETHOD_CREATEINDEXBUFFER,
	D3DMETHOD_CREATERENDERTARGET,
	D3DMETHOD_CREATEDEPTHSTENCILSURFACE,
	D3DMETHOD_UPDATESURFACE,
	D3DMETHOD_UPDATETEXTURE,
	D3DMETHOD_GETRENDERTARGETDATA,
	D3DMETHOD_GETFRONTBUFFERDATA,
	D3DMETHOD_STRETCHRECT,
	D3DMETHOD_COLORFILL,
	D3DMETHOD_CREATEOFFSCREENPLAINSURFACE,
	D3DMETHOD_SETRENDERTARGET,
	D3DMETHOD_GETRENDERTARGET,
	D3DMETHOD_SETDEPTHSTENCILSURFACE,
	D3DMETHOD_GETDEPTHSTENCILSURFACE,
	D3DMETHOD_BEGINSCENE,
	D3DMETHOD_ENDSCENE,
	D3DMETHOD_CLEAR,
	D3DMETHOD_SETTRANSFORM,
	D3DMETHOD_GETTRANSFORM,
	D3DMETHOD_MULTIPLYTRANSFORM,
	D3DMETHOD_SETVIEWPORT,
	D3DMETHOD_GETVIEWPORT,
	D3DMETHOD_SETMATERIAL,
	D3DMETHOD_GETMATERIAL,
	D3DMETHOD_SETLIGHT,
	D3DMETHOD_GETLIGHT,
	D3DMETHOD_LIGHTENABLE,
	D3DMETHOD_GETLIGHTENABLE,
	D3DMETHOD_SETCLIPPLANE,
	D3DMETHOD_GETCLIPPLANE,
	D3DMETHOD_SETRENDERSTATE,
	D3DMETHOD_GETRENDERSTATE,
	D3DMETHOD_CREATESTATEBLOCK,
	D3DMETHOD_BEGINSTATEBLOCK,
	D3DMETHOD_ENDSTATEBLOCK,
	D3DMETHOD_SETCLIPSTATUS,
	D3DMETHOD_GETCLIPSTATUS,
	D3DMETHOD_GETTEXTURE,
	D3DMETHOD_SETTEXTURE,
	D3DMETHOD_GETTEXTURESTAGESTATE,
	D3DMETHOD_SETTEXTURESTAGESTATE,
	D3DMETHOD_GETSAMPLERSTATE,
	D3DMETHOD_SETSAMPLERSTATE,
	D3DMETHOD_VALIDATEDEVICE,
	D3DMETHOD_SETPALETTEENTRIES,
	D3DMETHOD_GETPALETTEENTRIES,
	D3DMETHOD_SETCURRENTTEXTUREPALETTE,
	D3DMETHOD_GETCURRENTTEXTUREPALETTE,
	D3DMETHOD_SETSCISSORRECT,
	D3DMETHOD_GETSCISSORRECT,
	D3DMETHOD_SETSOFTWAREVERTEXPROCESSING,
	D3DMETHOD_GETSOFTWAREVERTEXPROCESSING,
	D3DMETHOD_SETNPATCHMODE,
	D3DMETHOD_GETNPATCHMODE,
	D3DMETHOD_DRAWPRIMITIVE,
	D3DMETHOD_DRAWINDEXEDPRIMITIVE,
	D3DMETHOD_DRAWPRIMITIVEUP,
	D3DMETHOD_DRAWINDEXEDPRIMITIVEUP,
	D3DMETHOD_PROCESSVERTICES,
	D3DMETHOD_CREATEVERTEXDECLARATION,
	D3DMETHOD_SETVERTEXDECLARATION,
	D3DMETHOD_GETVERTEXDECLARATION,
	D3DMETHOD_SETFVF,
	D3DMETHOD_GETFVF,
	D3DMETHOD_CREATEVERTEXSHADER,
	D3DMETHOD_SETVERTEXSHADER,
	D3DMETHOD_GETVERTEXSHADER,
	D3DMETHOD_SETVERTEXSHADERCONSTANTF,
	D3DMETHOD_GETVERTEXSHADERCONSTANTF,
	D3DMETHOD_SETVERTEXSHADERCONSTANTI,
	D3DMETHOD_GETVERTEXSHADERCONSTANTI,
	D3DMETHOD_SETVERTEXSHADERCONSTANTB,
	D3DMETHOD_GETVERTEXSHADERCONSTANTB,
	D3DMETHOD_SETSTREAMSOURCE,
	D3DMETHOD_GETSTREAMSOURCE,
	D3DMETHOD_SETSTREAMSOURCEFREQ,
	D3DMETHOD_GETSTREAMSOURCEFREQ,
	D3DMETHOD_SETINDICES,
	D3DMETHOD_GETINDICES,
	D3DMETHOD_CREATEPIXELSHADER,
	D3DMETHOD_SETPIXELSHADER,
	D3DMETHOD_GETPIXELSHADER,
	D3DMETHOD_SETPIXELSHADERCONSTANTF,
	D3DMETHOD_GETPIXELSHADERCONSTANTF,
	D3DMETHOD_SETPIXELSHADERCONSTANTI,
	D3DMETHOD_GETPIXELSHADERCONSTANTI,
	D3DMETHOD_SETPIXELSHADERCONSTANTB,
	D3DMETHOD_GETPIXELSHADERCONSTANTB,
	D3DMETHOD_DRAWRECTPATCH,
	D3DMETHOD_DRAWTRIPATCH,
	D3DMETHOD_DELETEPATCH,
	D3DMETHOD_CREATEQUERY
};

class SAMPFUNCS_API SFRender
{
	SFPluginInfo*							m_PluginInfo;

public:
	SFRender(SFPluginInfo* pluginInfo);
	SFPluginInfo*							getPluginInfo(void);

	void									registerD3DCallback(eDirect3DDeviceMethods eMethod, void* pFunc);
	IDirect3DDevice9						*getD3DDevice(void);
	IDirect3DDevice9						*getOrigD3DDevice(void);
	D3DPRESENT_PARAMETERS					*getPresentationParameters(void);
	HRESULT									BeginRender(void);
	HRESULT									EndRender(void);
	void									ARGB_To_A_R_G_B(D3DCOLOR D3DColor, uint8_t &byteA, uint8_t &byteR, uint8_t &byteG, uint8_t &byteB);
	void									DrawLine(int iX_1, int iY_1, int iX_2, int iY_2, int iW, D3DCOLOR D3DColor);
	void									DrawBox(int iX, int iY, int iW, int iH, D3DCOLOR D3DColor);
	void									DrawBorderedBox(int iX, int iY, int iW, int iH, D3DCOLOR D3DColor, int iBorderSize, D3DCOLOR D3DBorderColor);
	stFontInfo								*CreateNewFont(char *pszFontName, int iFontHeight, DWORD dwCreateFlags);
	bool									ReleaseFont(stFontInfo *pFont);
	void									DrawPolygon(int iX, int iY, int iW, int iH, float iRot, int iCorners, D3DCOLOR D3DColor);
	stTextureInfo							*LoadTextureFromFile(char *pszFile);
	stTextureInfo							*LoadTextureFromResource(int iResourceID, D3DCOLOR D3DColorKey);
	stTextureInfo							*LoadTextureFromMemory(LPCVOID pSrcData, UINT SrcDataSize);
	bool									ReleaseTexture(stTextureInfo *pTextureInfo);
	void									DrawTexture(stTextureInfo *pTexture, int iX, int iY, int iW, int iH, float iRot, D3DCOLOR D3DColor);
	HRESULT									Begin(D3DPRIMITIVETYPE D3DType);
	HRESULT									End(void);
	HRESULT									Color(D3DCOLOR dwColor);
	HRESULT									Vertex(float fX, float fY);
	void									TexCoord(float fX, float fY);
	void									BindTexture(IDirect3DTexture9 *pTexture);
	void									SetRenderState(D3DRENDERSTATETYPE D3DState, DWORD dwValue);
	bool									CanDraw(void);
};

#endif // SFRENDER_H__
