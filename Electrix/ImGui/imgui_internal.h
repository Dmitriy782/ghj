// dear imgui, v1.69 WIP
// (internal structures/api)

// You may use this file to debug, understand or extend ImGui features but we don't provide any guarantee of forward compatibility!
// Set:
//   #define IMGUI_DEFINE_MATH_OPERATORS

#pragma once

#ifndef IMGUI_VERSION
#error Must include imgui.h before imgui_internal.h
#endif

#include <stdio.h>       
#include <stdlib.h>           
#include <math.h>               
#include <limits.h>       

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4251)                     
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"                  
#pragma clang diagnostic ignored "-Wmissing-prototypes"               
#pragma clang diagnostic ignored "-Wold-style-cast"
#if __has_warning("-Wzero-as-null-pointer-constant")
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#if __has_warning("-Wdouble-promotion")
#pragma clang diagnostic ignored "-Wdouble-promotion"
#endif
#endif

struct ImRect;                           
struct ImDrawDataBuilder;                 
struct ImDrawListSharedData;              
struct ImGuiColorMod;                           
struct ImGuiColumnData;                   
struct ImGuiColumnsSet;                   
struct ImGuiContext;                   
struct ImGuiGroupData;                   
struct ImGuiInputTextState;                  
struct ImGuiItemHoveredDataBackup;        
struct ImGuiMenuColumns;                    
struct ImGuiNavMoveResult;                  
struct ImGuiNextWindowData;             
struct ImGuiPopupRef;                    
struct ImGuiSettingsHandler;                 
struct ImGuiStyleMod;                           
struct ImGuiTabBar;                      
struct ImGuiTabItem;                         
struct ImGuiWindow;                     
struct ImGuiWindowTempData;                             
struct ImGuiWindowSettings;                                

typedef int ImGuiLayoutType;                       
typedef int ImGuiButtonFlags;                     
typedef int ImGuiDragFlags;                        
typedef int ImGuiItemFlags;                        
typedef int ImGuiItemStatusFlags;            
typedef int ImGuiNavHighlightFlags;        
typedef int ImGuiNavDirSourceFlags;        
typedef int ImGuiNavMoveFlags;                   
typedef int ImGuiSeparatorFlags;                
typedef int ImGuiSliderFlags;                    
typedef int ImGuiTextFlags;                        

namespace ImStb
{

#undef STB_TEXTEDIT_STRING
#undef STB_TEXTEDIT_CHARTYPE
#define STB_TEXTEDIT_STRING             ImGuiInputTextState
#define STB_TEXTEDIT_CHARTYPE           ImWchar
#define STB_TEXTEDIT_GETWIDTH_NEWLINE   -1.0f
#define STB_TEXTEDIT_UNDOSTATECOUNT     99
#define STB_TEXTEDIT_UNDOCHARCOUNT      999
#include "imstb_textedit.h"

}   

#ifndef GImGui
extern IMGUI_API ImGuiContext* GImGui;       
#endif

#define IM_PI           3.14159265358979323846f
#ifdef _WIN32
#define IM_NEWLINE      "\r\n"                     
#else
#define IM_NEWLINE      "\n"
#endif

#define IMGUI_DEBUG_LOG(_FMT,...)       printf("[%05d] " _FMT, GImGui->FrameCount, __VA_ARGS__)
#define IM_STATIC_ASSERT(_COND)         typedef char static_assertion_##__line__[(_COND)?1:-1]
#define IM_F32_TO_INT8_UNBOUND(_VAL)    ((int)((_VAL) * 255.0f + ((_VAL)>=0 ? 0.5f : -0.5f)))       
#define IM_F32_TO_INT8_SAT(_VAL)        ((int)(ImSaturate(_VAL) * 255.0f + 0.5f))                   

#ifdef _MSC_VER
#define IMGUI_CDECL __cdecl
#else
#define IMGUI_CDECL
#endif

IMGUI_API int           ImTextStrToUtf8(char* buf, int buf_size, const ImWchar * in_text, const ImWchar * in_text_end);           
IMGUI_API int           ImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end);                  
IMGUI_API int           ImTextStrFromUtf8(ImWchar * buf, int buf_size, const char* in_text, const char* in_text_end, const char** in_remaining = NULL);        
IMGUI_API int           ImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end);                                    
IMGUI_API int           ImTextCountUtf8BytesFromChar(const char* in_text, const char* in_text_end);                                  
IMGUI_API int           ImTextCountUtf8BytesFromStr(const ImWchar * in_text, const ImWchar * in_text_end);                            

IMGUI_API ImU32         ImHashData(const void* data, size_t data_size, ImU32 seed = 0);
IMGUI_API ImU32         ImHashStr(const char* data, size_t data_size, ImU32 seed = 0);
IMGUI_API void* ImFileLoadToMemory(const char* filename, const char* file_open_mode, size_t * out_file_size = NULL, int padding_bytes = 0);
IMGUI_API FILE * ImFileOpen(const char* filename, const char* file_open_mode);
static inline bool      ImCharIsBlankA(char c) { return c == ' ' || c == '\t'; }
static inline bool      ImCharIsBlankW(unsigned int c) { return c == ' ' || c == '\t' || c == 0x3000; }
static inline bool      ImIsPowerOfTwo(int v) { return v != 0 && (v & (v - 1)) == 0; }
static inline int       ImUpperPowerOfTwo(int v) { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16; v++; return v; }
#define ImQsort         qsort
#ifndef IMGUI_DISABLE_OBSOLETE_FUNCTIONS
static inline ImU32     ImHash(const void* data, int size, ImU32 seed = 0) { return size ? ImHashData(data, (size_t)size, seed) : ImHashStr((const char*)data, 0, seed); }      
#endif

IMGUI_API ImVec2        ImLineClosestPoint(const ImVec2 & a, const ImVec2 & b, const ImVec2 & p);
IMGUI_API bool          ImTriangleContainsPoint(const ImVec2 & a, const ImVec2 & b, const ImVec2 & c, const ImVec2 & p);
IMGUI_API ImVec2        ImTriangleClosestPoint(const ImVec2 & a, const ImVec2 & b, const ImVec2 & c, const ImVec2 & p);
IMGUI_API void          ImTriangleBarycentricCoords(const ImVec2 & a, const ImVec2 & b, const ImVec2 & c, const ImVec2 & p, float& out_u, float& out_v, float& out_w);
IMGUI_API ImGuiDir      ImGetDirQuadrantFromDelta(float dx, float dy);

IMGUI_API int           ImStricmp(const char* str1, const char* str2);
IMGUI_API int           ImStrnicmp(const char* str1, const char* str2, size_t count);
IMGUI_API void          ImStrncpy(char* dst, const char* src, size_t count);
IMGUI_API char* ImStrdup(const char* str);
IMGUI_API char* ImStrdupcpy(char* dst, size_t * p_dst_size, const char* str);
IMGUI_API const char* ImStrchrRange(const char* str_begin, const char* str_end, char c);
IMGUI_API int           ImStrlenW(const ImWchar * str);
IMGUI_API const char* ImStreolRange(const char* str, const char* str_end);                  
IMGUI_API const ImWchar* ImStrbolW(const ImWchar * buf_mid_line, const ImWchar * buf_begin);     
IMGUI_API const char* ImStristr(const char* haystack, const char* haystack_end, const char* needle, const char* needle_end);
IMGUI_API void          ImStrTrimBlanks(char* str);
IMGUI_API int           ImFormatString(char* buf, size_t buf_size, const char* fmt, ...) IM_FMTARGS(3);
IMGUI_API int           ImFormatStringV(char* buf, size_t buf_size, const char* fmt, va_list args) IM_FMTLIST(3);
IMGUI_API const char* ImParseFormatFindStart(const char* format);
IMGUI_API const char* ImParseFormatFindEnd(const char* format);
IMGUI_API const char* ImParseFormatTrimDecorations(const char* format, char* buf, size_t buf_size);
IMGUI_API int           ImParseFormatPrecision(const char* format, int default_value);

#ifdef IMGUI_DEFINE_MATH_OPERATORS
static inline ImVec2 operator*(const ImVec2 & lhs, const float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }
static inline ImVec2 operator/(const ImVec2 & lhs, const float rhs) { return ImVec2(lhs.x / rhs, lhs.y / rhs); }
static inline ImVec2 operator+(const ImVec2 & lhs, const ImVec2 & rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
static inline ImVec2 operator-(const ImVec2 & lhs, const ImVec2 & rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
static inline ImVec2 operator*(const ImVec2 & lhs, const ImVec2 & rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
static inline ImVec2 operator/(const ImVec2 & lhs, const ImVec2 & rhs) { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
static inline ImVec2& operator+=(ImVec2 & lhs, const ImVec2 & rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
static inline ImVec2& operator-=(ImVec2 & lhs, const ImVec2 & rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
static inline ImVec2& operator*=(ImVec2 & lhs, const float rhs) { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
static inline ImVec2& operator/=(ImVec2 & lhs, const float rhs) { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
static inline ImVec4 operator+(const ImVec4 & lhs, const ImVec4 & rhs) { return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
static inline ImVec4 operator-(const ImVec4 & lhs, const ImVec4 & rhs) { return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
static inline ImVec4 operator*(const ImVec4 & lhs, const ImVec4 & rhs) { return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }
#endif

#ifndef IMGUI_DISABLE_MATH_FUNCTIONS
static inline float  ImFabs(float x) { return fabsf(x); }
static inline float  ImSqrt(float x) { return sqrtf(x); }
static inline float  ImPow(float x, float y) { return powf(x, y); }
static inline double ImPow(double x, double y) { return pow(x, y); }
static inline float  ImFmod(float x, float y) { return fmodf(x, y); }
static inline double ImFmod(double x, double y) { return fmod(x, y); }
static inline float  ImCos(float x) { return cosf(x); }
static inline float  ImSin(float x) { return sinf(x); }
static inline float  ImAcos(float x) { return acosf(x); }
static inline float  ImAtan2(float y, float x) { return atan2f(y, x); }
static inline double ImAtof(const char* s) { return atof(s); }
static inline float  ImFloorStd(float x) { return floorf(x); }                          
static inline float  ImCeil(float x) { return ceilf(x); }
#endif
template<typename T> static inline T ImMin(T lhs, T rhs) { return lhs < rhs ? lhs : rhs; }
template<typename T> static inline T ImMax(T lhs, T rhs) { return lhs >= rhs ? lhs : rhs; }
template<typename T> static inline T ImClamp(T v, T mn, T mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }
template<typename T> static inline T ImLerp(T a, T b, float t) { return (T)(a + (b - a) * t); }
template<typename T> static inline void ImSwap(T & a, T & b) { T tmp = a; a = b; b = tmp; }
template<typename T> static inline T ImAddClampOverflow(T a, T b, T mn, T mx) { if (b < 0 && (a < mn - b)) return mn; if (b > 0 && (a > mx - b)) return mx; return a + b; }
template<typename T> static inline T ImSubClampOverflow(T a, T b, T mn, T mx) { if (b > 0 && (a < mn + b)) return mn; if (b < 0 && (a > mx + b)) return mx; return a - b; }
static inline ImVec2 ImMin(const ImVec2 & lhs, const ImVec2 & rhs) { return ImVec2(lhs.x < rhs.x ? lhs.x : rhs.x, lhs.y < rhs.y ? lhs.y : rhs.y); }
static inline ImVec2 ImMax(const ImVec2 & lhs, const ImVec2 & rhs) { return ImVec2(lhs.x >= rhs.x ? lhs.x : rhs.x, lhs.y >= rhs.y ? lhs.y : rhs.y); }
static inline ImVec2 ImClamp(const ImVec2 & v, const ImVec2 & mn, ImVec2 mx) { return ImVec2((v.x < mn.x) ? mn.x : (v.x > mx.x) ? mx.x : v.x, (v.y < mn.y) ? mn.y : (v.y > mx.y) ? mx.y : v.y); }
static inline ImVec2 ImLerp(const ImVec2 & a, const ImVec2 & b, float t) { return ImVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t); }
static inline ImVec2 ImLerp(const ImVec2 & a, const ImVec2 & b, const ImVec2 & t) { return ImVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y); }
static inline ImVec4 ImLerp(const ImVec4 & a, const ImVec4 & b, float t) { return ImVec4(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t, a.w + (b.w - a.w) * t); }
static inline float  ImSaturate(float f) { return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f; }
static inline float  ImLengthSqr(const ImVec2 & lhs) { return lhs.x* lhs.x + lhs.y * lhs.y; }
static inline float  ImLengthSqr(const ImVec4 & lhs) { return lhs.x* lhs.x + lhs.y * lhs.y + lhs.z * lhs.z + lhs.w * lhs.w; }
static inline float  ImInvLength(const ImVec2 & lhs, float fail_value) { float d = lhs.x * lhs.x + lhs.y * lhs.y; if (d > 0.0f) return 1.0f / ImSqrt(d); return fail_value; }
static inline float  ImFloor(float f) { return (float)(int)f; }
static inline ImVec2 ImFloor(const ImVec2 & v) { return ImVec2((float)(int)v.x, (float)(int)v.y); }
static inline int    ImModPositive(int a, int b) { return (a + b) % b; }
static inline float  ImDot(const ImVec2 & a, const ImVec2 & b) { return a.x* b.x + a.y * b.y; }
static inline ImVec2 ImRotate(const ImVec2 & v, float cos_a, float sin_a) { return ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a); }
static inline float  ImLinearSweep(float current, float target, float speed) { if (current < target) return ImMin(current + speed, target); if (current > target) return ImMax(current - speed, target); return current; }
static inline ImVec2 ImMul(const ImVec2 & lhs, const ImVec2 & rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }

struct ImBoolVector
{
    ImVector<int>   Storage;
    ImBoolVector() { }
    void            Resize(int sz) { Storage.resize((sz + 31) >> 5); memset(Storage.Data, 0, (size_t)Storage.Size * sizeof(Storage.Data[0])); }
    void            Clear() { Storage.clear(); }
    bool            GetBit(int n) const { int off = (n >> 5); int mask = 1 << (n & 31); return (Storage[off] & mask) != 0; }
    void            SetBit(int n, bool v) { int off = (n >> 5); int mask = 1 << (n & 31); if (v) Storage[off] |= mask; else Storage[off] &= ~mask; }
};

typedef int ImPoolIdx;
template<typename T>
struct IMGUI_API ImPool
{
    ImVector<T>     Data;         
    ImGuiStorage    Map;         
    ImPoolIdx       FreeIdx;         

    ImPool() { FreeIdx = 0; }
    ~ImPool() { Clear(); }
    T* GetByKey(ImGuiID key) { int idx = Map.GetInt(key, -1); return (idx != -1) ? &Data[idx] : NULL; }
    T * GetByIndex(ImPoolIdx n) { return &Data[n]; }
    ImPoolIdx   GetIndex(const T * p) const { IM_ASSERT(p >= Data.Data && p < Data.Data + Data.Size); return (ImPoolIdx)(p - Data.Data); }
    T * GetOrAddByKey(ImGuiID key) { int* p_idx = Map.GetIntRef(key, -1); if (*p_idx != -1) return &Data[*p_idx]; *p_idx = FreeIdx; return Add(); }
    bool        Contains(const T * p) const { return (p >= Data.Data && p < Data.Data + Data.Size); }
    void        Clear() { for (int n = 0; n < Map.Data.Size; n++) { int idx = Map.Data[n].val_i; if (idx != -1) Data[idx].~T(); } Map.Clear(); Data.clear(); FreeIdx = 0; }
    T* Add() { int idx = FreeIdx; if (idx == Data.Size) { Data.resize(Data.Size + 1); FreeIdx++; } else { FreeIdx = *(int*)& Data[idx]; } IM_PLACEMENT_NEW(&Data[idx]) T(); return &Data[idx]; }
    void        Remove(ImGuiID key, const T * p) { Remove(key, GetIndex(p)); }
    void        Remove(ImGuiID key, ImPoolIdx idx) { Data[idx].~T(); *(int*)& Data[idx] = FreeIdx; FreeIdx = idx; Map.SetInt(key, -1); }
    void        Reserve(int capacity) { Data.reserve(capacity); Map.Data.reserve(capacity); }
    int         GetSize() const { return Data.Size; }
};

enum ImGuiButtonFlags_
{
    ImGuiButtonFlags_None = 0,
    ImGuiButtonFlags_Repeat = 1 << 0,      
    ImGuiButtonFlags_PressedOnClickRelease = 1 << 1,                   
    ImGuiButtonFlags_PressedOnClick = 1 << 2,          
    ImGuiButtonFlags_PressedOnRelease = 1 << 3,          
    ImGuiButtonFlags_PressedOnDoubleClick = 1 << 4,          
    ImGuiButtonFlags_FlattenChildren = 1 << 5,            
    ImGuiButtonFlags_AllowItemOverlap = 1 << 6,                     
    ImGuiButtonFlags_DontClosePopups = 1 << 7,            
    ImGuiButtonFlags_Disabled = 1 << 8,     
    ImGuiButtonFlags_AlignTextBaseLine = 1 << 9,                            
    ImGuiButtonFlags_NoKeyModifiers = 1 << 10,          
    ImGuiButtonFlags_NoHoldingActiveID = 1 << 11,           
    ImGuiButtonFlags_PressedOnDragDropHold = 1 << 12,                     
    ImGuiButtonFlags_NoNavFocus = 1 << 13         
};

enum ImGuiSliderFlags_
{
    ImGuiSliderFlags_None = 0,
    ImGuiSliderFlags_Vertical = 1 << 0
};

enum ImGuiDragFlags_
{
    ImGuiDragFlags_None = 0,
    ImGuiDragFlags_Vertical = 1 << 0
};

enum ImGuiColumnsFlags_
{
    ImGuiColumnsFlags_None = 0,
    ImGuiColumnsFlags_NoBorder = 1 << 0,      
    ImGuiColumnsFlags_NoResize = 1 << 1,           
    ImGuiColumnsFlags_NoPreserveWidths = 1 << 2,          
    ImGuiColumnsFlags_NoForceWithinWindow = 1 << 3,          
    ImGuiColumnsFlags_GrowParentContentsSize = 1 << 4                          
};

enum ImGuiSelectableFlagsPrivate_
{
    ImGuiSelectableFlags_NoHoldingActiveID = 1 << 10,
    ImGuiSelectableFlags_PressedOnClick = 1 << 11,
    ImGuiSelectableFlags_PressedOnRelease = 1 << 12,
    ImGuiSelectableFlags_DrawFillAvailWidth = 1 << 13
};

enum ImGuiSeparatorFlags_
{
    ImGuiSeparatorFlags_None = 0,
    ImGuiSeparatorFlags_Horizontal = 1 << 0,                  
    ImGuiSeparatorFlags_Vertical = 1 << 1
};

enum ImGuiItemFlags_
{
    ImGuiItemFlags_NoTabStop = 1 << 0,   
    ImGuiItemFlags_ButtonRepeat = 1 << 1,                   
    ImGuiItemFlags_Disabled = 1 << 2,                 
    ImGuiItemFlags_NoNav = 1 << 3,   
    ImGuiItemFlags_NoNavDefaultFocus = 1 << 4,   
    ImGuiItemFlags_SelectableDontClosePopup = 1 << 5,             
    ImGuiItemFlags_Default_ = 0
};

enum ImGuiItemStatusFlags_
{
    ImGuiItemStatusFlags_None = 0,
    ImGuiItemStatusFlags_HoveredRect = 1 << 0,
    ImGuiItemStatusFlags_HasDisplayRect = 1 << 1,
    ImGuiItemStatusFlags_Edited = 1 << 2                       

#ifdef IMGUI_ENABLE_TEST_ENGINE
    ,   
    ImGuiItemStatusFlags_Openable = 1 << 10,  
    ImGuiItemStatusFlags_Opened = 1 << 11,  
    ImGuiItemStatusFlags_Checkable = 1 << 12,  
    ImGuiItemStatusFlags_Checked = 1 << 13   
#endif
};

enum ImGuiTextFlags_
{
    ImGuiTextFlags_None = 0,
    ImGuiTextFlags_NoWidthForLargeClippedText = 1 << 0
};

enum ImGuiLayoutType_
{
    ImGuiLayoutType_Horizontal = 0,
    ImGuiLayoutType_Vertical = 1
};

enum ImGuiLogType
{
    ImGuiLogType_None = 0,
    ImGuiLogType_TTY,
    ImGuiLogType_File,
    ImGuiLogType_Buffer,
    ImGuiLogType_Clipboard
};

enum ImGuiAxis
{
    ImGuiAxis_None = -1,
    ImGuiAxis_X = 0,
    ImGuiAxis_Y = 1
};

enum ImGuiPlotType
{
    ImGuiPlotType_Lines,
    ImGuiPlotType_Histogram
};

enum ImGuiInputSource
{
    ImGuiInputSource_None = 0,
    ImGuiInputSource_Mouse,
    ImGuiInputSource_Nav,
    ImGuiInputSource_NavKeyboard,             
    ImGuiInputSource_NavGamepad,     
    ImGuiInputSource_COUNT
};

enum ImGuiInputReadMode
{
    ImGuiInputReadMode_Down,
    ImGuiInputReadMode_Pressed,
    ImGuiInputReadMode_Released,
    ImGuiInputReadMode_Repeat,
    ImGuiInputReadMode_RepeatSlow,
    ImGuiInputReadMode_RepeatFast
};

enum ImGuiNavHighlightFlags_
{
    ImGuiNavHighlightFlags_None = 0,
    ImGuiNavHighlightFlags_TypeDefault = 1 << 0,
    ImGuiNavHighlightFlags_TypeThin = 1 << 1,
    ImGuiNavHighlightFlags_AlwaysDraw = 1 << 2,                   
    ImGuiNavHighlightFlags_NoRounding = 1 << 3
};

enum ImGuiNavDirSourceFlags_
{
    ImGuiNavDirSourceFlags_None = 0,
    ImGuiNavDirSourceFlags_Keyboard = 1 << 0,
    ImGuiNavDirSourceFlags_PadDPad = 1 << 1,
    ImGuiNavDirSourceFlags_PadLStick = 1 << 2
};

enum ImGuiNavMoveFlags_
{
    ImGuiNavMoveFlags_None = 0,
    ImGuiNavMoveFlags_LoopX = 1 << 0,          
    ImGuiNavMoveFlags_LoopY = 1 << 1,
    ImGuiNavMoveFlags_WrapX = 1 << 2,                     
    ImGuiNavMoveFlags_WrapY = 1 << 3,            
    ImGuiNavMoveFlags_AllowCurrentNavId = 1 << 4,                                                  
    ImGuiNavMoveFlags_AlsoScoreVisibleSet = 1 << 5                  
};

enum ImGuiNavForward
{
    ImGuiNavForward_None,
    ImGuiNavForward_ForwardQueued,
    ImGuiNavForward_ForwardActive
};

enum ImGuiNavLayer
{
    ImGuiNavLayer_Main = 0,       
    ImGuiNavLayer_Menu = 1,         
    ImGuiNavLayer_COUNT
};

enum ImGuiPopupPositionPolicy
{
    ImGuiPopupPositionPolicy_Default,
    ImGuiPopupPositionPolicy_ComboBox
};

struct ImVec1
{
    float   x;
    ImVec1() { x = 0.0f; }
    ImVec1(float _x) { x = _x; }
};


struct IMGUI_API ImRect
{
    ImVec2      Min;     
    ImVec2      Max;     

    ImRect() : Min(FLT_MAX, FLT_MAX), Max(-FLT_MAX, -FLT_MAX) {}
    ImRect(const ImVec2& min, const ImVec2& max) : Min(min), Max(max) {}
    ImRect(const ImVec4& v) : Min(v.x, v.y), Max(v.z, v.w) {}
    ImRect(float x1, float y1, float x2, float y2) : Min(x1, y1), Max(x2, y2) {}

    ImVec2      GetCenter() const { return ImVec2((Min.x + Max.x) * 0.5f, (Min.y + Max.y) * 0.5f); }
    ImVec2      GetSize() const { return ImVec2(Max.x - Min.x, Max.y - Min.y); }
    float       GetWidth() const { return Max.x - Min.x; }
    float       GetHeight() const { return Max.y - Min.y; }
    ImVec2      GetTL() const { return Min; }                    
    ImVec2      GetTR() const { return ImVec2(Max.x, Min.y); }   
    ImVec2      GetBL() const { return ImVec2(Min.x, Max.y); }   
    ImVec2      GetBR() const { return Max; }                    
    bool        Contains(const ImVec2 & p) const { return p.x >= Min.x && p.y >= Min.y && p.x < Max.x && p.y < Max.y; }
    bool        Contains(const ImRect & r) const { return r.Min.x >= Min.x && r.Min.y >= Min.y && r.Max.x <= Max.x && r.Max.y <= Max.y; }
    bool        Overlaps(const ImRect & r) const { return r.Min.y <  Max.y&& r.Max.y >  Min.y&& r.Min.x <  Max.x&& r.Max.x >  Min.x; }
    void        Add(const ImVec2 & p) { if (Min.x > p.x)     Min.x = p.x;     if (Min.y > p.y)     Min.y = p.y;     if (Max.x < p.x)     Max.x = p.x;     if (Max.y < p.y)     Max.y = p.y; }
    void        Add(const ImRect & r) { if (Min.x > r.Min.x) Min.x = r.Min.x; if (Min.y > r.Min.y) Min.y = r.Min.y; if (Max.x < r.Max.x) Max.x = r.Max.x; if (Max.y < r.Max.y) Max.y = r.Max.y; }
    void        Expand(const float amount) { Min.x -= amount;   Min.y -= amount;   Max.x += amount;   Max.y += amount; }
    void        Expand(const ImVec2 & amount) { Min.x -= amount.x; Min.y -= amount.y; Max.x += amount.x; Max.y += amount.y; }
    void        Translate(const ImVec2 & d) { Min.x += d.x; Min.y += d.y; Max.x += d.x; Max.y += d.y; }
    void        TranslateX(float dx) { Min.x += dx; Max.x += dx; }
    void        TranslateY(float dy) { Min.y += dy; Max.y += dy; }
    void        ClipWith(const ImRect & r) { Min = ImMax(Min, r.Min); Max = ImMin(Max, r.Max); }                                     
    void        ClipWithFull(const ImRect & r) { Min = ImClamp(Min, r.Min, r.Max); Max = ImClamp(Max, r.Min, r.Max); }         
    void        Floor() { Min.x = (float)(int)Min.x; Min.y = (float)(int)Min.y; Max.x = (float)(int)Max.x; Max.y = (float)(int)Max.y; }
    bool        IsInverted() const { return Min.x > Max.x || Min.y > Max.y; }
};

struct ImGuiColorMod
{
    ImGuiCol    Col;
    ImVec4      BackupValue;
};

struct ImGuiStyleMod
{
    ImGuiStyleVar   VarIdx;
    union { int BackupInt[2]; float BackupFloat[2]; };
    ImGuiStyleMod(ImGuiStyleVar idx, int v) { VarIdx = idx; BackupInt[0] = v; }
    ImGuiStyleMod(ImGuiStyleVar idx, float v) { VarIdx = idx; BackupFloat[0] = v; }
    ImGuiStyleMod(ImGuiStyleVar idx, ImVec2 v) { VarIdx = idx; BackupFloat[0] = v.x; BackupFloat[1] = v.y; }
};

struct ImGuiGroupData
{
    ImVec2      BackupCursorPos;
    ImVec2      BackupCursorMaxPos;
    ImVec1      BackupIndent;
    ImVec1      BackupGroupOffset;
    ImVec2      BackupCurrentLineSize;
    float       BackupCurrentLineTextBaseOffset;
    ImGuiID     BackupActiveIdIsAlive;
    bool        BackupActiveIdPreviousFrameIsAlive;
    bool        AdvanceCursor;
};

struct IMGUI_API ImGuiMenuColumns
{
    float       Spacing;
    float       Width, NextWidth;
    float       Pos[3], NextWidths[3];

    ImGuiMenuColumns();
    void        Update(int count, float spacing, bool clear);
    float       DeclColumns(float w0, float w1, float w2);
    float       CalcExtraSpace(float avail_w);
};

struct IMGUI_API ImGuiInputTextState
{
    ImGuiID                 ID;                           
    int                     CurLenW, CurLenA;                             
    ImVector<ImWchar>       TextW;                                       
    ImVector<char>          TextA;                                  
    ImVector<char>          InitialTextA;                       
    bool                    TextAIsValid;                                 
    int                     BufCapacityA;              
    float                   ScrollX;                  
    ImStb::STB_TexteditState Stb;                      
    float                   CursorAnim;                           
    bool                    CursorFollow;                        
    bool                    SelectedAllMouseLock;                 

    ImGuiInputTextFlags     UserFlags;
    ImGuiInputTextCallback  UserCallback;
    void* UserCallbackData;

    ImGuiInputTextState() { memset(this, 0, sizeof(*this)); }
    void                ClearFreeMemory() { TextW.clear(); TextA.clear(); InitialTextA.clear(); }
    void                CursorAnimReset() { CursorAnim = -0.30f; }                                               
    void                CursorClamp() { Stb.cursor = ImMin(Stb.cursor, CurLenW); Stb.select_start = ImMin(Stb.select_start, CurLenW); Stb.select_end = ImMin(Stb.select_end, CurLenW); }
    bool                HasSelection() const { return Stb.select_start != Stb.select_end; }
    void                ClearSelection() { Stb.select_start = Stb.select_end = Stb.cursor; }
    void                SelectAll() { Stb.select_start = 0; Stb.cursor = Stb.select_end = CurLenW; Stb.has_preferred_x = 0; }
    int                 GetUndoAvailCount() const { return Stb.undostate.undo_point; }
    int                 GetRedoAvailCount() const { return STB_TEXTEDIT_UNDOSTATECOUNT - Stb.undostate.redo_point; }
    void                OnKeyPressed(int key);                 
};

struct ImGuiWindowSettings
{
    char* Name;
    ImGuiID     ID;
    ImVec2      Pos;
    ImVec2      Size;
    bool        Collapsed;

    ImGuiWindowSettings() { Name = NULL; ID = 0; Pos = Size = ImVec2(0, 0); Collapsed = false; }
};

struct ImGuiSettingsHandler
{
    const char* TypeName;                 
    ImGuiID     TypeHash;           
    void* (*ReadOpenFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, const char* name);                         
    void        (*ReadLineFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, void* entry, const char* line);            
    void        (*WriteAllFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, ImGuiTextBuffer* out_buf);            
    void* UserData;

    ImGuiSettingsHandler() { memset(this, 0, sizeof(*this)); }
};

struct ImGuiPopupRef
{
    ImGuiID             PopupId;           
    ImGuiWindow* Window;                     
    ImGuiWindow* ParentWindow;      
    int                 OpenFrameCount;    
    ImGuiID             OpenParentId;                         
    ImVec2              OpenPopupPos;               
    ImVec2              OpenMousePos;                
};

struct ImGuiColumnData
{
    float               OffsetNorm;                    
    float               OffsetNormBeforeResize;
    ImGuiColumnsFlags   Flags;                
    ImRect              ClipRect;

    ImGuiColumnData() { OffsetNorm = OffsetNormBeforeResize = 0.0f; Flags = 0; }
};

struct ImGuiColumnsSet
{
    ImGuiID             ID;
    ImGuiColumnsFlags   Flags;
    bool                IsFirstFrame;
    bool                IsBeingResized;
    int                 Current;
    int                 Count;
    float               MinX, MaxX;
    float               LineMinY, LineMaxY;
    float               StartPosY;             
    float               StartMaxPosX;          
    ImVector<ImGuiColumnData> Columns;

    ImGuiColumnsSet() { Clear(); }
    void Clear()
    {
        ID = 0;
        Flags = 0;
        IsFirstFrame = false;
        IsBeingResized = false;
        Current = 0;
        Count = 1;
        MinX = MaxX = 0.0f;
        LineMinY = LineMaxY = 0.0f;
        StartPosY = 0.0f;
        StartMaxPosX = 0.0f;
        Columns.clear();
    }
};

struct IMGUI_API ImDrawListSharedData
{
    ImVec2          TexUvWhitePixel;                   
    ImFont* Font;                              
    float           FontSize;                           
    float           CurveTessellationTol;
    ImVec4          ClipRectFullscreen;            

    ImVec2          CircleVtx12[12];

    ImDrawListSharedData();
};

struct ImDrawDataBuilder
{
    ImVector<ImDrawList*>   Layers[2];                

    void Clear() { for (int n = 0; n < IM_ARRAYSIZE(Layers); n++) Layers[n].resize(0); }
    void ClearFreeMemory() { for (int n = 0; n < IM_ARRAYSIZE(Layers); n++) Layers[n].clear(); }
    IMGUI_API void FlattenIntoSingleLayer();
};

struct ImGuiNavMoveResult
{
    ImGuiID       ID;             
    ImGuiID       SelectScopeId;       
    ImGuiWindow* Window;          
    float         DistBox;             
    float         DistCenter;          
    float         DistAxial;
    ImRect        RectRel;              

    ImGuiNavMoveResult() { Clear(); }
    void Clear() { ID = SelectScopeId = 0; Window = NULL; DistBox = DistCenter = DistAxial = FLT_MAX; RectRel = ImRect(); }
};

struct ImGuiNextWindowData
{
    ImGuiCond               PosCond;
    ImGuiCond               SizeCond;
    ImGuiCond               ContentSizeCond;
    ImGuiCond               CollapsedCond;
    ImGuiCond               SizeConstraintCond;
    ImGuiCond               FocusCond;
    ImGuiCond               BgAlphaCond;
    ImVec2                  PosVal;
    ImVec2                  PosPivotVal;
    ImVec2                  SizeVal;
    ImVec2                  ContentSizeVal;
    bool                    CollapsedVal;
    ImRect                  SizeConstraintRect;
    ImGuiSizeCallback       SizeCallback;
    void* SizeCallbackUserData;
    float                   BgAlphaVal;
    ImVec2                  MenuBarOffsetMinVal;                          

    ImGuiNextWindowData()
    {
        PosCond = SizeCond = ContentSizeCond = CollapsedCond = SizeConstraintCond = FocusCond = BgAlphaCond = 0;
        PosVal = PosPivotVal = SizeVal = ImVec2(0.0f, 0.0f);
        ContentSizeVal = ImVec2(0.0f, 0.0f);
        CollapsedVal = false;
        SizeConstraintRect = ImRect();
        SizeCallback = NULL;
        SizeCallbackUserData = NULL;
        BgAlphaVal = FLT_MAX;
        MenuBarOffsetMinVal = ImVec2(0.0f, 0.0f);
    }

    void    Clear()
    {
        PosCond = SizeCond = ContentSizeCond = CollapsedCond = SizeConstraintCond = FocusCond = BgAlphaCond = 0;
    }
};

struct ImGuiTabBarSortItem
{
    int             Index;
    float           Width;
};

struct ImGuiTabBarRef
{
    ImGuiTabBar* Ptr;                                        
    int             IndexInMainPool;

    ImGuiTabBarRef(ImGuiTabBar* ptr) { Ptr = ptr; IndexInMainPool = -1; }
    ImGuiTabBarRef(int index_in_main_pool) { Ptr = NULL; IndexInMainPool = index_in_main_pool; }
};

struct ImGuiContext
{
    bool                    Initialized;
    bool                    FrameScopeActive;                         
    bool                    FrameScopePushedImplicitWindow;           
    bool                    FontAtlasOwnedByContext;                         
    ImGuiIO                 IO;
    ImGuiStyle              Style;
    ImFont* Font;                                      
    float                   FontSize;                                       
    float                   FontBaseSize;                                 
    ImDrawListSharedData    DrawListSharedData;

    double                  Time;
    int                     FrameCount;
    int                     FrameCountEnded;
    int                     FrameCountRendered;
    ImVector<ImGuiWindow*>  Windows;                                    
    ImVector<ImGuiWindow*>  WindowsFocusOrder;                          
    ImVector<ImGuiWindow*>  WindowsSortBuffer;
    ImVector<ImGuiWindow*>  CurrentWindowStack;
    ImGuiStorage            WindowsById;
    int                     WindowsActiveCount;
    ImGuiWindow* CurrentWindow;                         
    ImGuiWindow* HoveredWindow;                          
    ImGuiWindow* HoveredRootWindow;                         
    ImGuiID                 HoveredId;                            
    bool                    HoveredIdAllowOverlap;
    ImGuiID                 HoveredIdPreviousFrame;
    float                   HoveredIdTimer;                         
    float                   HoveredIdNotActiveTimer;                       
    ImGuiID                 ActiveId;                             
    ImGuiID                 ActiveIdPreviousFrame;
    ImGuiID                 ActiveIdIsAlive;                                        
    float                   ActiveIdTimer;
    bool                    ActiveIdIsJustActivated;                     
    bool                    ActiveIdAllowOverlap;                               
    bool                    ActiveIdHasBeenPressed;                                      
    bool                    ActiveIdHasBeenEdited;                             
    bool                    ActiveIdPreviousFrameIsAlive;
    bool                    ActiveIdPreviousFrameHasBeenEdited;
    int                     ActiveIdAllowNavDirFlags;                           
    int                     ActiveIdBlockNavInputFlags;
    ImVec2                  ActiveIdClickOffset;                            
    ImGuiWindow* ActiveIdWindow;
    ImGuiWindow* ActiveIdPreviousFrameWindow;
    ImGuiInputSource        ActiveIdSource;                           
    ImGuiID                 LastActiveId;                               
    float                   LastActiveIdTimer;                                
    ImVec2                  LastValidMousePos;
    ImGuiWindow* MovingWindow;                                           
    ImVector<ImGuiColorMod> ColorModifiers;                        
    ImVector<ImGuiStyleMod> StyleModifiers;                        
    ImVector<ImFont*>       FontStack;                             
    ImVector<ImGuiPopupRef> OpenPopupStack;                          
    ImVector<ImGuiPopupRef> BeginPopupStack;                              
    ImGuiNextWindowData     NextWindowData;                         
    bool                    NextTreeNodeOpenVal;                    
    ImGuiCond               NextTreeNodeOpenCond;

    ImGuiWindow* NavWindow;                                  
    ImGuiID                 NavId;                                  
    ImGuiID                 NavActivateId;                                     
    ImGuiID                 NavActivateDownId;                        
    ImGuiID                 NavActivatePressedId;                     
    ImGuiID                 NavInputId;                               
    ImGuiID                 NavJustTabbedId;                         
    ImGuiID                 NavJustMovedToId;                             
    ImGuiID                 NavJustMovedToSelectScopeId;                    
    ImGuiID                 NavNextActivateId;                         
    ImGuiInputSource        NavInputSource;                                  
    ImRect                  NavScoringRectScreen;                              
    int                     NavScoringCount;                       
    ImGuiWindow* NavWindowingTarget;                                 
    ImGuiWindow* NavWindowingTargetAnim;                        
    ImGuiWindow* NavWindowingList;
    float                   NavWindowingTimer;
    float                   NavWindowingHighlightAlpha;
    bool                    NavWindowingToggleLayer;
    ImGuiNavLayer           NavLayer;                                                
    int                     NavIdTabCounter;                           
    bool                    NavIdIsAlive;                                  
    bool                    NavMousePosDirty;                                      
    bool                    NavDisableHighlight;                                       
    bool                    NavDisableMouseHover;                              
    bool                    NavAnyRequest;                          
    bool                    NavInitRequest;                              
    bool                    NavInitRequestFromMove;
    ImGuiID                 NavInitResultId;
    ImRect                  NavInitResultRectRel;
    bool                    NavMoveFromClampedRefRect;                              
    bool                    NavMoveRequest;                          
    ImGuiNavMoveFlags       NavMoveRequestFlags;
    ImGuiNavForward         NavMoveRequestForward;                               
    ImGuiDir                NavMoveDir, NavMoveDirLast;                     
    ImGuiDir                NavMoveClipDir;
    ImGuiNavMoveResult      NavMoveResultLocal;                       
    ImGuiNavMoveResult      NavMoveResultLocalVisibleSet;                     
    ImGuiNavMoveResult      NavMoveResultOther;                             

    ImGuiWindow* FocusRequestCurrWindow;             
    ImGuiWindow* FocusRequestNextWindow;             
    int                     FocusRequestCurrCounterAll;                                   
    int                     FocusRequestCurrCounterTab;                   
    int                     FocusRequestNextCounterAll;             
    int                     FocusRequestNextCounterTab;          
    bool                    FocusTabPressed;                    

    ImDrawData              DrawData;                                     
    ImDrawDataBuilder       DrawDataBuilder;
    float                   DimBgRatio;                                       
    ImDrawList              BackgroundDrawList;                       
    ImDrawList              ForegroundDrawList;                                        
    ImGuiMouseCursor        MouseCursor;

    bool                    DragDropActive;
    bool                    DragDropWithinSourceOrTarget;
    ImGuiDragDropFlags      DragDropSourceFlags;
    int                     DragDropSourceFrameCount;
    int                     DragDropMouseButton;
    ImGuiPayload            DragDropPayload;
    ImRect                  DragDropTargetRect;
    ImGuiID                 DragDropTargetId;
    ImGuiDragDropFlags      DragDropAcceptFlags;
    float                   DragDropAcceptIdCurrRectSurface;                
    ImGuiID                 DragDropAcceptIdCurr;                          
    ImGuiID                 DragDropAcceptIdPrev;                                  
    int                     DragDropAcceptFrameCount;                      
    ImVector<unsigned char> DragDropPayloadBufHeap;                   
    unsigned char           DragDropPayloadBufLocal[8];              

    ImPool<ImGuiTabBar>             TabBars;
    ImGuiTabBar* CurrentTabBar;
    ImVector<ImGuiTabBarRef>        CurrentTabBarStack;
    ImVector<ImGuiTabBarSortItem>   TabSortByWidthBuffer;

    ImGuiInputTextState     InputTextState;
    ImFont                  InputTextPasswordFont;
    ImGuiID                 ScalarAsInputTextId;                         
    ImGuiColorEditFlags     ColorEditOptions;                          
    ImVec4                  ColorPickerRef;
    bool                    DragCurrentAccumDirty;
    float                   DragCurrentAccum;                               
    float                   DragSpeedDefaultRatio;                      
    ImVec2                  ScrollbarClickDeltaToGrabCenter;                  
    int                     TooltipOverrideCount;
    ImVector<char>          PrivateClipboard;                          

    ImGuiID                 MultiSelectScopeId;

    ImVec2                  PlatformImePos;                                 
    ImVec2                  PlatformImeLastPos;

    bool                           SettingsLoaded;
    float                          SettingsDirtyTimer;                   
    ImGuiTextBuffer                SettingsIniData;                 
    ImVector<ImGuiSettingsHandler> SettingsHandlers;                 
    ImVector<ImGuiWindowSettings>  SettingsWindows;                            

    bool                    LogEnabled;
    ImGuiLogType            LogType;
    FILE* LogFile;                                   
    ImGuiTextBuffer         LogBuffer;                                            
    float                   LogLinePosY;
    bool                    LogLineFirstItem;
    int                     LogDepthRef;
    int                     LogDepthToExpand;
    int                     LogDepthToExpandDefault;                        

    float                   FramerateSecPerFrame[120];                     
    int                     FramerateSecPerFrameIdx;
    float                   FramerateSecPerFrameAccum;
    int                     WantCaptureMouseNextFrame;                 
    int                     WantCaptureKeyboardNextFrame;
    int                     WantTextInputNextFrame;
    char                    TempBuffer[1024 * 3 + 1];                  

    ImGuiContext(ImFontAtlas* shared_font_atlas) : BackgroundDrawList(NULL), ForegroundDrawList(NULL)
    {
        Initialized = false;
        FrameScopeActive = FrameScopePushedImplicitWindow = false;
        Font = NULL;
        FontSize = FontBaseSize = 0.0f;
        FontAtlasOwnedByContext = shared_font_atlas ? false : true;
        IO.Fonts = shared_font_atlas ? shared_font_atlas : IM_NEW(ImFontAtlas)();

        Time = 0.0f;
        FrameCount = 0;
        FrameCountEnded = FrameCountRendered = -1;
        WindowsActiveCount = 0;
        CurrentWindow = NULL;
        HoveredWindow = NULL;
        HoveredRootWindow = NULL;
        HoveredId = 0;
        HoveredIdAllowOverlap = false;
        HoveredIdPreviousFrame = 0;
        HoveredIdTimer = HoveredIdNotActiveTimer = 0.0f;
        ActiveId = 0;
        ActiveIdPreviousFrame = 0;
        ActiveIdIsAlive = 0;
        ActiveIdTimer = 0.0f;
        ActiveIdIsJustActivated = false;
        ActiveIdAllowOverlap = false;
        ActiveIdHasBeenPressed = false;
        ActiveIdHasBeenEdited = false;
        ActiveIdPreviousFrameIsAlive = false;
        ActiveIdPreviousFrameHasBeenEdited = false;
        ActiveIdAllowNavDirFlags = 0x00;
        ActiveIdBlockNavInputFlags = 0x00;
        ActiveIdClickOffset = ImVec2(-1, -1);
        ActiveIdWindow = ActiveIdPreviousFrameWindow = NULL;
        ActiveIdSource = ImGuiInputSource_None;
        LastActiveId = 0;
        LastActiveIdTimer = 0.0f;
        LastValidMousePos = ImVec2(0.0f, 0.0f);
        MovingWindow = NULL;
        NextTreeNodeOpenVal = false;
        NextTreeNodeOpenCond = 0;

        NavWindow = NULL;
        NavId = NavActivateId = NavActivateDownId = NavActivatePressedId = NavInputId = 0;
        NavJustTabbedId = NavJustMovedToId = NavJustMovedToSelectScopeId = NavNextActivateId = 0;
        NavInputSource = ImGuiInputSource_None;
        NavScoringRectScreen = ImRect();
        NavScoringCount = 0;
        NavWindowingTarget = NavWindowingTargetAnim = NavWindowingList = NULL;
        NavWindowingTimer = NavWindowingHighlightAlpha = 0.0f;
        NavWindowingToggleLayer = false;
        NavLayer = ImGuiNavLayer_Main;
        NavIdTabCounter = INT_MAX;
        NavIdIsAlive = false;
        NavMousePosDirty = false;
        NavDisableHighlight = true;
        NavDisableMouseHover = false;
        NavAnyRequest = false;
        NavInitRequest = false;
        NavInitRequestFromMove = false;
        NavInitResultId = 0;
        NavMoveFromClampedRefRect = false;
        NavMoveRequest = false;
        NavMoveRequestFlags = 0;
        NavMoveRequestForward = ImGuiNavForward_None;
        NavMoveDir = NavMoveDirLast = NavMoveClipDir = ImGuiDir_None;

        FocusRequestCurrWindow = FocusRequestNextWindow = NULL;
        FocusRequestCurrCounterAll = FocusRequestCurrCounterTab = INT_MAX;
        FocusRequestNextCounterAll = FocusRequestNextCounterTab = INT_MAX;
        FocusTabPressed = false;

        DimBgRatio = 0.0f;
        BackgroundDrawList._Data = &DrawListSharedData;
        BackgroundDrawList._OwnerName = "##Background";       
        ForegroundDrawList._Data = &DrawListSharedData;
        ForegroundDrawList._OwnerName = "##Foreground";       
        MouseCursor = ImGuiMouseCursor_Arrow;

        DragDropActive = DragDropWithinSourceOrTarget = false;
        DragDropSourceFlags = 0;
        DragDropSourceFrameCount = -1;
        DragDropMouseButton = -1;
        DragDropTargetId = 0;
        DragDropAcceptFlags = 0;
        DragDropAcceptIdCurrRectSurface = 0.0f;
        DragDropAcceptIdPrev = DragDropAcceptIdCurr = 0;
        DragDropAcceptFrameCount = -1;
        memset(DragDropPayloadBufLocal, 0, sizeof(DragDropPayloadBufLocal));

        CurrentTabBar = NULL;

        ScalarAsInputTextId = 0;
        ColorEditOptions = ImGuiColorEditFlags__OptionsDefault;
        DragCurrentAccumDirty = false;
        DragCurrentAccum = 0.0f;
        DragSpeedDefaultRatio = 1.0f / 100.0f;
        ScrollbarClickDeltaToGrabCenter = ImVec2(0.0f, 0.0f);
        TooltipOverrideCount = 0;

        MultiSelectScopeId = 0;

        PlatformImePos = PlatformImeLastPos = ImVec2(FLT_MAX, FLT_MAX);

        SettingsLoaded = false;
        SettingsDirtyTimer = 0.0f;

        LogEnabled = false;
        LogType = ImGuiLogType_None;
        LogFile = NULL;
        LogLinePosY = FLT_MAX;
        LogLineFirstItem = false;
        LogDepthRef = 0;
        LogDepthToExpand = LogDepthToExpandDefault = 2;

        memset(FramerateSecPerFrame, 0, sizeof(FramerateSecPerFrame));
        FramerateSecPerFrameIdx = 0;
        FramerateSecPerFrameAccum = 0.0f;
        WantCaptureMouseNextFrame = WantCaptureKeyboardNextFrame = WantTextInputNextFrame = -1;
        memset(TempBuffer, 0, sizeof(TempBuffer));
    }
};

struct IMGUI_API ImGuiWindowTempData
{
    ImVec2                  CursorPos;
    ImVec2                  CursorPosPrevLine;
    ImVec2                  CursorStartPos;                
    ImVec2                  CursorMaxPos;                                  
    ImVec2                  CurrentLineSize;
    float                   CurrentLineTextBaseOffset;
    ImVec2                  PrevLineSize;
    float                   PrevLineTextBaseOffset;
    int                     TreeDepth;
    ImU32                   TreeDepthMayJumpToParentOnPop;         
    ImGuiID                 LastItemId;
    ImGuiItemStatusFlags    LastItemStatusFlags;
    ImRect                  LastItemRect;             
    ImRect                  LastItemDisplayRect;             
    ImGuiNavLayer           NavLayerCurrent;                
    int                     NavLayerCurrentMask;              
    int                     NavLayerActiveMask;               
    int                     NavLayerActiveMaskNext;           
    bool                    NavHideHighlightOneFrame;
    bool                    NavHasScroll;                    
    bool                    MenuBarAppending;          
    ImVec2                  MenuBarOffset;                                            
    ImVector<ImGuiWindow*>  ChildWindows;
    ImGuiStorage* StateStorage;
    ImGuiLayoutType         LayoutType;
    ImGuiLayoutType         ParentLayoutType;                 
    int                     FocusCounterAll;                        
    int                     FocusCounterTab;                  

    ImGuiItemFlags          ItemFlags;                   
    float                   ItemWidth;                               
    float                   TextWrapPos;                 
    ImVector<ImGuiItemFlags>ItemFlagsStack;
    ImVector<float>         ItemWidthStack;
    ImVector<float>         TextWrapPosStack;
    ImVector<ImGuiGroupData>GroupStack;
    short                   StackSizesBackup[6];           

    ImVec1                  Indent;                             
    ImVec1                  GroupOffset;
    ImVec1                  ColumnsOffset;                                      
    ImGuiColumnsSet* ColumnsSet;                

    ImGuiWindowTempData()
    {
        CursorPos = CursorPosPrevLine = CursorStartPos = CursorMaxPos = ImVec2(0.0f, 0.0f);
        CurrentLineSize = PrevLineSize = ImVec2(0.0f, 0.0f);
        CurrentLineTextBaseOffset = PrevLineTextBaseOffset = 0.0f;
        TreeDepth = 0;
        TreeDepthMayJumpToParentOnPop = 0x00;
        LastItemId = 0;
        LastItemStatusFlags = 0;
        LastItemRect = LastItemDisplayRect = ImRect();
        NavLayerActiveMask = NavLayerActiveMaskNext = 0x00;
        NavLayerCurrent = ImGuiNavLayer_Main;
        NavLayerCurrentMask = (1 << ImGuiNavLayer_Main);
        NavHideHighlightOneFrame = false;
        NavHasScroll = false;
        MenuBarAppending = false;
        MenuBarOffset = ImVec2(0.0f, 0.0f);
        StateStorage = NULL;
        LayoutType = ParentLayoutType = ImGuiLayoutType_Vertical;
        FocusCounterAll = FocusCounterTab = -1;

        ItemFlags = ImGuiItemFlags_Default_;
        ItemWidth = 0.0f;
        TextWrapPos = -1.0f;
        memset(StackSizesBackup, 0, sizeof(StackSizesBackup));

        Indent = ImVec1(0.0f);
        GroupOffset = ImVec1(0.0f);
        ColumnsOffset = ImVec1(0.0f);
        ColumnsSet = NULL;
    }
};

struct IMGUI_API ImGuiWindow
{
    char* Name;
    ImGuiID                 ID;                                   
    ImGuiWindowFlags        Flags;                                 
    ImVec2                  Pos;                                      
    ImVec2                  Size;                                       
    ImVec2                  SizeFull;                               
    ImVec2                  SizeFullAtLastBegin;                                         
    ImVec2                  SizeContents;                                           
    ImVec2                  SizeContentsExplicit;                         
    ImVec2                  WindowPadding;                             
    float                   WindowRounding;                            
    float                   WindowBorderSize;                           
    int                     NameBufLen;                                   
    ImGuiID                 MoveId;                               
    ImGuiID                 ChildId;                                             
    ImVec2                  Scroll;
    ImVec2                  ScrollTarget;                                             
    ImVec2                  ScrollTargetCenterRatio;                               
    ImVec2                  ScrollbarSizes;                            
    bool                    ScrollbarX, ScrollbarY;
    bool                    Active;                                    
    bool                    WasActive;
    bool                    WriteAccessed;                                
    bool                    Collapsed;                                  
    bool                    WantCollapseToggle;
    bool                    SkipItems;                                        
    bool                    Appearing;                                     
    bool                    Hidden;                                     
    bool                    HasCloseButton;                                
    signed char             ResizeBorderHeld;                             
    short                   BeginCount;                                           
    short                   BeginOrderWithinParent;                          
    short                   BeginOrderWithinContext;                           
    ImGuiID                 PopupId;                                                
    int                     AutoFitFramesX, AutoFitFramesY;
    bool                    AutoFitOnlyGrows;
    int                     AutoFitChildAxises;
    ImGuiDir                AutoPosLastDirection;
    int                     HiddenFramesRegular;                      
    int                     HiddenFramesForResize;                                
    ImGuiCond               SetWindowPosAllowFlags;                    
    ImGuiCond               SetWindowSizeAllowFlags;                   
    ImGuiCond               SetWindowCollapsedAllowFlags;              
    ImVec2                  SetWindowPosVal;                                        
    ImVec2                  SetWindowPosPivot;                                    

    ImGuiWindowTempData     DC;                                                      
    ImVector<ImGuiID>       IDStack;                                           
    ImRect                  ClipRect;                                        
    ImRect                  OuterRectClipped;                               
    ImRect                  InnerMainRect, InnerClipRect;
    ImRect                  ContentsRegionRect;                                        
    int                     LastFrameActive;                           
    float                   ItemWidthDefault;
    ImGuiMenuColumns        MenuColumns;                              
    ImGuiStorage            StateStorage;
    ImVector<ImGuiColumnsSet> ColumnsStorage;
    float                   FontWindowScale;                        
    int                     SettingsIdx;                                        

    ImDrawList* DrawList;                                          
    ImDrawList              DrawListInst;
    ImGuiWindow* ParentWindow;                                       
    ImGuiWindow* RootWindow;                                     
    ImGuiWindow* RootWindowForTitleBarHighlight;                     
    ImGuiWindow* RootWindowForNav;                               

    ImGuiWindow* NavLastChildNavWindow;                                             
    ImGuiID                 NavLastIds[ImGuiNavLayer_COUNT];             
    ImRect                  NavRectRel[ImGuiNavLayer_COUNT];          

public:
    ImGuiWindow(ImGuiContext* context, const char* name);
    ~ImGuiWindow();

    ImGuiID     GetID(const char* str, const char* str_end = NULL);
    ImGuiID     GetID(const void* ptr);
    ImGuiID     GetIDNoKeepAlive(const char* str, const char* str_end = NULL);
    ImGuiID     GetIDNoKeepAlive(const void* ptr);
    ImGuiID     GetIDFromRectangle(const ImRect& r_abs);

    ImRect      Rect() const { return ImRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y); }
    float       CalcFontSize() const { return GImGui->FontBaseSize* FontWindowScale; }
    float       TitleBarHeight() const { return (Flags & ImGuiWindowFlags_NoTitleBar) ? 0.0f : CalcFontSize() + GImGui->Style.FramePadding.y * 2.0f; }
    ImRect      TitleBarRect() const { return ImRect(Pos, ImVec2(Pos.x + SizeFull.x, Pos.y + TitleBarHeight())); }
    float       MenuBarHeight() const { return (Flags & ImGuiWindowFlags_MenuBar) ? DC.MenuBarOffset.y + CalcFontSize() + GImGui->Style.FramePadding.y * 2.0f : 0.0f; }
    ImRect      MenuBarRect() const { float y1 = Pos.y + TitleBarHeight(); return ImRect(Pos.x, y1, Pos.x + SizeFull.x, y1 + MenuBarHeight()); }
};

struct ImGuiItemHoveredDataBackup
{
    ImGuiID                 LastItemId;
    ImGuiItemStatusFlags    LastItemStatusFlags;
    ImRect                  LastItemRect;
    ImRect                  LastItemDisplayRect;

    ImGuiItemHoveredDataBackup() { Backup(); }
    void Backup() { ImGuiWindow* window = GImGui->CurrentWindow; LastItemId = window->DC.LastItemId; LastItemStatusFlags = window->DC.LastItemStatusFlags; LastItemRect = window->DC.LastItemRect; LastItemDisplayRect = window->DC.LastItemDisplayRect; }
    void Restore() const { ImGuiWindow* window = GImGui->CurrentWindow; window->DC.LastItemId = LastItemId; window->DC.LastItemStatusFlags = LastItemStatusFlags; window->DC.LastItemRect = LastItemRect; window->DC.LastItemDisplayRect = LastItemDisplayRect; }
};

enum ImGuiTabBarFlagsPrivate_
{
    ImGuiTabBarFlags_DockNode = 1 << 20,                        
    ImGuiTabBarFlags_IsFocused = 1 << 21,
    ImGuiTabBarFlags_SaveSettings = 1 << 22                        
};

enum ImGuiTabItemFlagsPrivate_
{
    ImGuiTabItemFlags_NoCloseButton = 1 << 20                  
};

struct ImGuiTabItem
{
    ImGuiID             ID;
    ImGuiTabItemFlags   Flags;
    int                 LastFrameVisible;
    int                 LastFrameSelected;                      
    int                 NameOffset;                     
    float               Offset;                       
    float               Width;                     
    float               WidthContents;                  

    ImGuiTabItem() { ID = Flags = 0; LastFrameVisible = LastFrameSelected = -1; NameOffset = -1; Offset = Width = WidthContents = 0.0f; }
};

struct ImGuiTabBar
{
    ImVector<ImGuiTabItem> Tabs;
    ImGuiID             ID;                           
    ImGuiID             SelectedTabId;            
    ImGuiID             NextSelectedTabId;
    ImGuiID             VisibleTabId;                       
    int                 CurrFrameVisible;
    int                 PrevFrameVisible;
    ImRect              BarRect;
    float               ContentsHeight;
    float               OffsetMax;                    
    float               OffsetNextTab;                        
    float               ScrollingAnim;
    float               ScrollingTarget;
    ImGuiTabBarFlags    Flags;
    ImGuiID             ReorderRequestTabId;
    int                 ReorderRequestDir;
    bool                WantLayout;
    bool                VisibleTabWasSubmitted;
    short               LastTabItemIdx;           
    ImVec2              FramePadding;                  
    ImGuiTextBuffer     TabsNames;                          

    ImGuiTabBar();
    int                 GetTabOrder(const ImGuiTabItem* tab) const { return Tabs.index_from_ptr(tab); }
    const char* GetTabName(const ImGuiTabItem* tab) const
    {
        IM_ASSERT(tab->NameOffset != -1 && tab->NameOffset < TabsNames.Buf.Size);
        return TabsNames.Buf.Data + tab->NameOffset;
    }
};

namespace ImGui
{
    inline    ImGuiWindow* GetCurrentWindowRead() { ImGuiContext& g = *GImGui; return g.CurrentWindow; }
    inline    ImGuiWindow* GetCurrentWindow() { ImGuiContext& g = *GImGui; g.CurrentWindow->WriteAccessed = true; return g.CurrentWindow; }
    IMGUI_API ImGuiWindow* FindWindowByID(ImGuiID id);
    IMGUI_API ImGuiWindow* FindWindowByName(const char* name);
    IMGUI_API void          FocusWindow(ImGuiWindow* window);
    IMGUI_API void          FocusPreviousWindowIgnoringOne(ImGuiWindow* ignore_window);
    IMGUI_API void          BringWindowToFocusFront(ImGuiWindow* window);
    IMGUI_API void          BringWindowToDisplayFront(ImGuiWindow* window);
    IMGUI_API void          BringWindowToDisplayBack(ImGuiWindow* window);
    IMGUI_API void          UpdateWindowParentAndRootLinks(ImGuiWindow* window, ImGuiWindowFlags flags, ImGuiWindow* parent_window);
    IMGUI_API ImVec2        CalcWindowExpectedSize(ImGuiWindow* window);
    IMGUI_API bool          IsWindowChildOf(ImGuiWindow* window, ImGuiWindow* potential_parent);
    IMGUI_API bool          IsWindowNavFocusable(ImGuiWindow* window);
    IMGUI_API void          SetWindowScrollX(ImGuiWindow* window, float new_scroll_x);
    IMGUI_API void          SetWindowScrollY(ImGuiWindow* window, float new_scroll_y);
    IMGUI_API float         GetWindowScrollMaxX(ImGuiWindow* window);
    IMGUI_API float         GetWindowScrollMaxY(ImGuiWindow* window);
    IMGUI_API ImRect        GetWindowAllowedExtentRect(ImGuiWindow* window);
    IMGUI_API void          SetWindowPos(ImGuiWindow* window, const ImVec2& pos, ImGuiCond cond);
    IMGUI_API void          SetWindowSize(ImGuiWindow* window, const ImVec2& size, ImGuiCond cond);
    IMGUI_API void          SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiCond cond);

    IMGUI_API void          SetCurrentFont(ImFont* font);
    inline ImFont* GetDefaultFont() { ImGuiContext& g = *GImGui; return g.IO.FontDefault ? g.IO.FontDefault : g.IO.Fonts->Fonts[0]; }

    IMGUI_API void          Initialize(ImGuiContext* context);
    IMGUI_API void          Shutdown(ImGuiContext* context);                      

    IMGUI_API void          UpdateHoveredWindowAndCaptureFlags();
    IMGUI_API void          StartMouseMovingWindow(ImGuiWindow* window);
    IMGUI_API void          UpdateMouseMovingWindowNewFrame();
    IMGUI_API void          UpdateMouseMovingWindowEndFrame();

    IMGUI_API void                  MarkIniSettingsDirty();
    IMGUI_API void                  MarkIniSettingsDirty(ImGuiWindow* window);
    IMGUI_API ImGuiWindowSettings* CreateNewWindowSettings(const char* name);
    IMGUI_API ImGuiWindowSettings* FindWindowSettings(ImGuiID id);
    IMGUI_API ImGuiWindowSettings* FindOrCreateWindowSettings(const char* name);
    IMGUI_API ImGuiSettingsHandler* FindSettingsHandler(const char* type_name);

    inline ImGuiID          GetItemID() { ImGuiContext& g = *GImGui; return g.CurrentWindow->DC.LastItemId; }
    inline ImGuiID          GetActiveID() { ImGuiContext& g = *GImGui; return g.ActiveId; }
    inline ImGuiID          GetFocusID() { ImGuiContext& g = *GImGui; return g.NavId; }
    IMGUI_API void          SetActiveID(ImGuiID id, ImGuiWindow* window);
    IMGUI_API void          SetFocusID(ImGuiID id, ImGuiWindow* window);
    IMGUI_API void          ClearActiveID();
    IMGUI_API ImGuiID       GetHoveredID();
    IMGUI_API void          SetHoveredID(ImGuiID id);
    IMGUI_API void          KeepAliveID(ImGuiID id);
    IMGUI_API void          MarkItemEdited(ImGuiID id);

    IMGUI_API void          ItemSize(const ImVec2& size, float text_offset_y = 0.0f);
    IMGUI_API void          ItemSize(const ImRect& bb, float text_offset_y = 0.0f);
    IMGUI_API bool          ItemAdd(const ImRect& bb, ImGuiID id, const ImRect* nav_bb = NULL);
    IMGUI_API bool          ItemHoverable(const ImRect& bb, ImGuiID id);
    IMGUI_API bool          IsClippedEx(const ImRect& bb, ImGuiID id, bool clip_even_when_logged);
    IMGUI_API bool          FocusableItemRegister(ImGuiWindow* window, ImGuiID id);         
    IMGUI_API void          FocusableItemUnregister(ImGuiWindow* window);
    IMGUI_API ImVec2        CalcItemSize(ImVec2 size, float default_x, float default_y);
    IMGUI_API float         CalcWrapWidthForPos(const ImVec2& pos, float wrap_pos_x);
    IMGUI_API void          PushMultiItemsWidths(int components, float width_full = 0.0f);
    IMGUI_API void          PushItemFlag(ImGuiItemFlags option, bool enabled);
    IMGUI_API void          PopItemFlag();

    IMGUI_API void          LogBegin(ImGuiLogType type, int auto_open_depth);                     
    IMGUI_API void          LogToBuffer(int auto_open_depth = -1);                   

    IMGUI_API void          OpenPopupEx(ImGuiID id);
    IMGUI_API void          ClosePopupToLevel(int remaining, bool apply_focus_to_window_under);
    IMGUI_API void          ClosePopupsOverWindow(ImGuiWindow* ref_window);
    IMGUI_API bool          IsPopupOpen(ImGuiID id);                   
    IMGUI_API bool          BeginPopupEx(ImGuiID id, ImGuiWindowFlags extra_flags);
    IMGUI_API void          BeginTooltipEx(ImGuiWindowFlags extra_flags, bool override_previous_tooltip = true);
    IMGUI_API ImGuiWindow* GetFrontMostPopupModal();
    IMGUI_API ImVec2        FindBestWindowPosForPopup(ImGuiWindow* window);
    IMGUI_API ImVec2        FindBestWindowPosForPopupEx(const ImVec2& ref_pos, const ImVec2& size, ImGuiDir* last_dir, const ImRect& r_outer, const ImRect& r_avoid, ImGuiPopupPositionPolicy policy = ImGuiPopupPositionPolicy_Default);

    IMGUI_API void          NavInitWindow(ImGuiWindow* window, bool force_reinit);
    IMGUI_API bool          NavMoveRequestButNoResultYet();
    IMGUI_API void          NavMoveRequestCancel();
    IMGUI_API void          NavMoveRequestForward(ImGuiDir move_dir, ImGuiDir clip_dir, const ImRect& bb_rel, ImGuiNavMoveFlags move_flags);
    IMGUI_API void          NavMoveRequestTryWrapping(ImGuiWindow* window, ImGuiNavMoveFlags move_flags);
    IMGUI_API float         GetNavInputAmount(ImGuiNavInput n, ImGuiInputReadMode mode);
    IMGUI_API ImVec2        GetNavInputAmount2d(ImGuiNavDirSourceFlags dir_sources, ImGuiInputReadMode mode, float slow_factor = 0.0f, float fast_factor = 0.0f);
    IMGUI_API int           CalcTypematicPressedRepeatAmount(float t, float t_prev, float repeat_delay, float repeat_rate);
    IMGUI_API void          ActivateItem(ImGuiID id);                              
    IMGUI_API void          SetNavID(ImGuiID id, int nav_layer);
    IMGUI_API void          SetNavIDWithRectRel(ImGuiID id, int nav_layer, const ImRect& rect_rel);

    inline bool             IsKeyPressedMap(ImGuiKey key, bool repeat = true) { const int key_index = GImGui->IO.KeyMap[key]; return (key_index >= 0) ? IsKeyPressed(key_index, repeat) : false; }
    inline bool             IsNavInputDown(ImGuiNavInput n) { return GImGui->IO.NavInputs[n] > 0.0f; }
    inline bool             IsNavInputPressed(ImGuiNavInput n, ImGuiInputReadMode mode) { return GetNavInputAmount(n, mode) > 0.0f; }
    inline bool             IsNavInputPressedAnyOfTwo(ImGuiNavInput n1, ImGuiNavInput n2, ImGuiInputReadMode mode) { return (GetNavInputAmount(n1, mode) + GetNavInputAmount(n2, mode)) > 0.0f; }

    IMGUI_API bool          BeginDragDropTargetCustom(const ImRect & bb, ImGuiID id);
    IMGUI_API void          ClearDragDrop();
    IMGUI_API bool          IsDragDropPayloadBeingAccepted();

    IMGUI_API void          BeginColumns(const char* str_id, int count, ImGuiColumnsFlags flags = 0);                
    IMGUI_API void          EndColumns();                                                               
    IMGUI_API void          PushColumnClipRect(int column_index = -1);

    IMGUI_API bool          BeginTabBarEx(ImGuiTabBar * tab_bar, const ImRect & bb, ImGuiTabBarFlags flags);
    IMGUI_API ImGuiTabItem * TabBarFindTabByID(ImGuiTabBar * tab_bar, ImGuiID tab_id);
    IMGUI_API void          TabBarRemoveTab(ImGuiTabBar * tab_bar, ImGuiID tab_id);
    IMGUI_API void          TabBarCloseTab(ImGuiTabBar * tab_bar, ImGuiTabItem * tab);
    IMGUI_API void          TabBarQueueChangeTabOrder(ImGuiTabBar * tab_bar, const ImGuiTabItem * tab, int dir);
    IMGUI_API bool          TabItemEx(ImGuiTabBar * tab_bar, const char* label, bool* p_open, ImGuiTabItemFlags flags);
    IMGUI_API ImVec2        TabItemCalcSize(const char* label, bool has_close_button);
    IMGUI_API void          TabItemBackground(ImDrawList * draw_list, const ImRect & bb, ImGuiTabItemFlags flags, ImU32 col);
    IMGUI_API bool          TabItemLabelAndCloseButton(ImDrawList * draw_list, const ImRect & bb, ImGuiTabItemFlags flags, ImVec2 frame_padding, const char* label, ImGuiID tab_id, ImGuiID close_button_id);

    IMGUI_API void          RenderText(ImVec2 pos, const char* text, const char* text_end = NULL, bool hide_text_after_hash = true);
    IMGUI_API void          RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width);
    IMGUI_API void          RenderTextClipped(const ImVec2 & pos_min, const ImVec2 & pos_max, const char* text, const char* text_end, const ImVec2 * text_size_if_known, const ImVec2 & align = ImVec2(0, 0), const ImRect * clip_rect = NULL);
    IMGUI_API void          RenderTextClippedEx(ImDrawList * draw_list, const ImVec2 & pos_min, const ImVec2 & pos_max, const char* text, const char* text_end, const ImVec2 * text_size_if_known, const ImVec2 & align = ImVec2(0, 0), const ImRect * clip_rect = NULL);
    IMGUI_API void          RenderFrame(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, bool border = true, float rounding = 0.0f);
    IMGUI_API void          RenderFrameBorder(ImVec2 p_min, ImVec2 p_max, float rounding = 0.0f);
    IMGUI_API void          RenderColorRectWithAlphaCheckerboard(ImVec2 p_min, ImVec2 p_max, ImU32 fill_col, float grid_step, ImVec2 grid_off, float rounding = 0.0f, int rounding_corners_flags = ~0);
    IMGUI_API void          RenderArrow(ImVec2 pos, ImGuiDir dir, float scale = 1.0f);
    IMGUI_API void          RenderBullet(ImVec2 pos);
    IMGUI_API void          RenderCheckMark(ImVec2 pos, ImU32 col, float sz);
    IMGUI_API void          RenderNavHighlight(const ImRect & bb, ImGuiID id, ImGuiNavHighlightFlags flags = ImGuiNavHighlightFlags_TypeDefault);   
    IMGUI_API const char* FindRenderedTextEnd(const char* text, const char* text_end = NULL);           
    IMGUI_API void          LogRenderedText(const ImVec2 * ref_pos, const char* text, const char* text_end = NULL);

    IMGUI_API void          RenderMouseCursor(ImDrawList * draw_list, ImVec2 pos, float scale, ImGuiMouseCursor mouse_cursor = ImGuiMouseCursor_Arrow);
    IMGUI_API void          RenderArrowPointingAt(ImDrawList * draw_list, ImVec2 pos, ImVec2 half_sz, ImGuiDir direction, ImU32 col);
    IMGUI_API void          RenderRectFilledRangeH(ImDrawList * draw_list, const ImRect & rect, ImU32 col, float x_start_norm, float x_end_norm, float rounding);
    IMGUI_API void          RenderPixelEllipsis(ImDrawList * draw_list, ImVec2 pos, int count, ImU32 col);

    IMGUI_API void          TextEx(const char* text, const char* text_end = NULL, ImGuiTextFlags flags = 0);
    IMGUI_API bool          ButtonEx(const char* label, const ImVec2 & size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0);
    IMGUI_API bool          CloseButton(ImGuiID id, const ImVec2 & pos, float radius);
    IMGUI_API bool          CollapseButton(ImGuiID id, const ImVec2 & pos);
    IMGUI_API bool          ArrowButtonEx(const char* str_id, ImGuiDir dir, ImVec2 size_arg, ImGuiButtonFlags flags);
    IMGUI_API void          Scrollbar(ImGuiAxis axis);
    IMGUI_API ImGuiID       GetScrollbarID(ImGuiWindow * window, ImGuiAxis axis);
    IMGUI_API void          VerticalSeparator();                                

    IMGUI_API bool          ButtonBehavior(const ImRect & bb, ImGuiID id, bool* out_hovered, bool* out_held, ImGuiButtonFlags flags = 0);
    IMGUI_API bool          DragBehavior(ImGuiID id, ImGuiDataType data_type, void* v, float v_speed, const void* v_min, const void* v_max, const char* format, float power, ImGuiDragFlags flags);
    IMGUI_API bool          SliderBehavior(const ImRect & bb, ImGuiID id, ImGuiDataType data_type, void* v, const void* v_min, const void* v_max, const char* format, float power, ImGuiSliderFlags flags, ImRect * out_grab_bb);
    IMGUI_API bool          SplitterBehavior(const ImRect & bb, ImGuiID id, ImGuiAxis axis, float* size1, float* size2, float min_size1, float min_size2, float hover_extend = 0.0f, float hover_visibility_delay = 0.0f);
    IMGUI_API bool          TreeNodeBehavior(ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end = NULL);
    IMGUI_API bool          TreeNodeBehaviorIsOpen(ImGuiID id, ImGuiTreeNodeFlags flags = 0);                                
    IMGUI_API void          TreePushRawID(ImGuiID id);

    template<typename T, typename SIGNED_T, typename FLOAT_T>   IMGUI_API bool  DragBehaviorT(ImGuiDataType data_type, T * v, float v_speed, T v_min, T v_max, const char* format, float power, ImGuiDragFlags flags);
    template<typename T, typename SIGNED_T, typename FLOAT_T>   IMGUI_API bool  SliderBehaviorT(const ImRect & bb, ImGuiID id, ImGuiDataType data_type, T * v, T v_min, T v_max, const char* format, float power, ImGuiSliderFlags flags, ImRect * out_grab_bb);
    template<typename T, typename FLOAT_T>                      IMGUI_API float SliderCalcRatioFromValueT(ImGuiDataType data_type, T v, T v_min, T v_max, float power, float linear_zero_pos);
    template<typename T, typename SIGNED_T>                     IMGUI_API T     RoundScalarWithFormatT(const char* format, ImGuiDataType data_type, T v);

    IMGUI_API bool          InputTextEx(const char* label, const char* hint, char* buf, int buf_size, const ImVec2 & size_arg, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    IMGUI_API bool          InputScalarAsWidgetReplacement(const ImRect & bb, ImGuiID id, const char* label, ImGuiDataType data_type, void* data_ptr, const char* format);

    IMGUI_API void          ColorTooltip(const char* text, const float* col, ImGuiColorEditFlags flags);
    IMGUI_API void          ColorEditOptionsPopup(const float* col, ImGuiColorEditFlags flags);
    IMGUI_API void          ColorPickerOptionsPopup(const float* ref_col, ImGuiColorEditFlags flags);

    IMGUI_API void          PlotEx(ImGuiPlotType plot_type, const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, ImVec2 frame_size);

    IMGUI_API void          ShadeVertsLinearColorGradientKeepAlpha(ImDrawList * draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImU32 col0, ImU32 col1);
    IMGUI_API void          ShadeVertsLinearUV(ImDrawList * draw_list, int vert_start_idx, int vert_end_idx, const ImVec2 & a, const ImVec2 & b, const ImVec2 & uv_a, const ImVec2 & uv_b, bool clamp);

}   

IMGUI_API bool              ImFontAtlasBuildWithStbTruetype(ImFontAtlas * atlas);
IMGUI_API void              ImFontAtlasBuildRegisterDefaultCustomRects(ImFontAtlas * atlas);
IMGUI_API void              ImFontAtlasBuildSetupFont(ImFontAtlas * atlas, ImFont * font, ImFontConfig * font_config, float ascent, float descent);
IMGUI_API void              ImFontAtlasBuildPackCustomRects(ImFontAtlas * atlas, void* stbrp_context_opaque);
IMGUI_API void              ImFontAtlasBuildFinish(ImFontAtlas * atlas);
IMGUI_API void              ImFontAtlasBuildMultiplyCalcLookupTable(unsigned char out_table[256], float in_multiply_factor);
IMGUI_API void              ImFontAtlasBuildMultiplyRectAlpha8(const unsigned char table[256], unsigned char* pixels, int x, int y, int w, int h, int stride);

#ifdef IMGUI_ENABLE_TEST_ENGINE
extern void                 ImGuiTestEngineHook_PreNewFrame(ImGuiContext * ctx);
extern void                 ImGuiTestEngineHook_PostNewFrame(ImGuiContext * ctx);
extern void                 ImGuiTestEngineHook_ItemAdd(ImGuiContext * ctx, const ImRect & bb, ImGuiID id);
extern void                 ImGuiTestEngineHook_ItemInfo(ImGuiContext * ctx, ImGuiID id, const char* label, ImGuiItemStatusFlags flags);
#define IMGUI_TEST_ENGINE_ITEM_INFO(_ID, _LABEL, _FLAGS)  ImGuiTestEngineHook_ItemInfo(&g, _ID, _LABEL, _FLAGS)      
#else
#define IMGUI_TEST_ENGINE_ITEM_INFO(_ID, _LABEL, _FLAGS)  do { } while (0)
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning (pop)
#endif