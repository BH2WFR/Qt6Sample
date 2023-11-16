#ifndef TERMINAL_BEAUTY_H__
#define TERMINAL_BEAUTY_H__


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
	#include <windows.h>
#endif // WIN32

#ifndef __cplusplus
	#include <stdbool.h>
#endif

#ifdef __cplusplus

#endif

#ifdef _MSC_VER
	#pragma warning( disable: 4003 4996 )
#endif

//*参考 Ansi 转义序列:  https://zh.wikipedia.org/wiki/ANSI%E8%BD%AC%E4%B9%89%E5%BA%8F%E5%88%97

//* printf/std::cout 的颜色控制代码, 用于界面美化
#define CBlack		"\033[30m"
#define CRed 		"\033[31m"
#define CGreen 		"\033[32m"
#define CBrown		"\033[33m"
#define CBlue		"\033[34m"
#define CPurple		"\033[35m"
#define CCyan		"\033[36m"
#define CWhite		"\033[37m"
#define CFrontColor(C)		"\033[38;5;"#C"m"
#define CFrontRGB(R, G, B)	"\033[38;2;" #R ";" #G ";" #B "m"

#define CLBlack		"\033[90m"
#define CLRed 		"\033[91m"
#define CLGreen 	"\033[92m"
#define CLYellow 	"\033[93m"
#define CLBlue 		"\033[94m"
#define CLPurple 	"\033[95m"
#define CLCyan 		"\033[96m"
#define CLWhite 	"\033[97m"


#define BBlack 		"\033[40m"
#define BRed 		"\033[41m"
#define BWhite 		"\033[47m"
#define CBackColor(C)		"\033[48;5;"#C"m"
#define CBackRGB(R, G, B)	"\033[38;2;" #R ";" #G ";" #B "m"


#define CTurn 		"\033[107;30m"	//白底黑字
#define CDefault 	"\033[0m"		//恢复默认

#define CBold		"\033[1m"
#define CWeak		"\033[2m"
#define CItalic		"\033[3m"
#define CUnderLine	"\033[4m"
#define CFlash		"\033[5m"
#define CQFlash		"\033[6m"
#define CInverse	"\033[7m"

#define NPRINTF(str, ...) 				printf(str "\n", __VA_ARGS__ )
#define PRINTF_COLOR(color, str, ...) 	printf(color str CDefault, __VA_ARGS__ )
#define CPRINTF(color, str, ... ) 		printf(color str CDefault, __VA_ARGS__ )
#define CNPRINTF(color, str, ... ) 		printf(color str CDefault "\n", __VA_ARGS__ )


//* 命令行窗口 光标控制
#define CUp(n)			"\033["#n"A"
#define CDown(n)		"\033["#n"B" // std::cout << CDown("2") 光标下移两行
#define CLeft(n)		"\033["#n"C"
#define CRight(n)		"\033["#n"D"
#define CNextLn(n)		"\033["#n"E"
#define CPrevLn(n)		"\033["#n"F"
#define CHorzPos(n)		"\033["#n"G"
#define CPos(x, y)		"\033["#x";"#y"H"
#define CEraseAfter		"\033[0J"
#define CEraseBefore	"\033[1J"
#define CEraseAll		"\033[2J" //清屏 但光标不动
#define CClearAll		"\033[3J" //清屏+删除缓存区
#define CEraseLine(n)	"\033["#n"K"
#define CScrollUp(n)	"\033["#n"S"
#define CScrollDn(n)	"\033["#n"T"
#define CSaveCursurPos	"\033[s"
#define CReadCursurPos  "\033[u"


//* 命令行窗口 和 输入流 相关宏
#define TERMINAL_UTF_8() 		system("chcp 65001")  //* 命令行用 utf-8
#define TERMINAL_CLEAR()		system("cls") //清屏
#define TERMINAL_TITLE(STR)		SetConsoleTitle(STR) //设置标题
#define TERMINAL_SIZE(X, Y) 	system("mode con cols=" #X "lines=" #Y)

#define TERMINAL_PAUSE()		system(pause)
#define TERMINAL_ABSORB()		{char ch; while((ch = getchar()) != '\n') continue;}

//* 程序调试使用
#define PRINT_ARGV(argc, argv) { printf("\n" CLYellow); \
		printf(CLYellow CBold "================= 程序参数显示 =================\n");\
		printf(CLGreen "argc:   " CLWhite "%d\n", argc); \
		printf(CLGreen "argv[  0]: " CLBlue "%.256s\n", argv[0]); \
		for(int i = 1; i < argc; i++) printf(CLYellow "argv[%3d]: " CLCyan "%.256s\n", i, argv[i]); \
		printf(CDefault "\n"); }



//* 错误提示
#ifdef __CUDA_RUNTIME_H__ //* CUDA 错误检查与退出机制
	#define CHK_CUDA_ERR(err, str, ...) \
	if(err != cudaSuccess){ printf(CLRed "\n=============== CUDA ERROR %d: " #str "\n   file: " __FILE__ "\n   func: %s\n   line: %d\n" CDefault , err, __func__, __LINE__);return __VA_ARGS__;}

	#define ABORT_CUDA_ERR(err, str) \
	if(err != cudaSuccess){ printf(CLRed "\n=============== CUDA ERROR %d: " #str "\n   file: " __FILE__ "\n   func: %s\n   line: %d\n" CDefault , err, __func__, __LINE__);exit(-1);}
#endif //__CUDA_RUNTIME_H__


#define PRINT_ERR(str) \
	printf(CLRed "\n=============== ERROR: " CLYellow #str CLRed "\n\
    file: " __FILE__ "\n\
    func: %s\n\
    line: %d\n\n" CDefault , \
		 __func__, __LINE__);

#define ABORT_ERR(str) {PRINT_ERR(str); exit(-1);}
#define ASMABORT_ERR(str) {PRINT_ERR(str); asm("exit;" );}


#define ABORT_NULLPTR(ptr, str) \
	{if(ptr == NULL) ABORT_ERR(str) }




//==== C++ 兼容代码
#ifdef __cplusplus
	
	#ifdef QT_VERSION //==== Qt 适配
		#ifdef QDEBUG_H //* qDebug() 懒人包
			#define QDEBUGNN qDebug().nospace().noquote() // 不输出空格和引号的 qDebug()
			#define QDEBUGN  qDebug().noquote() 		  // 不加引号的 qDebug()
		#endif
		
		#if (QT_VERSION >= QT_VERSION_CHECK(5,14,0)) // Qt 5.14.0 及以上版本 高分屏缩放率 自适应 (分数缩放比)
			#define _QT_HIGH_DPI_ROUNDING_POLICY() \
				QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
		#else //QT_VERSION
			#define _QT_HIGH_DPI_ROUNDING_POLICY()
		#endif //QT_VERSION
		
		//* 高分屏支持, Qt [5.6.0, 6.x.x), 需要在 main 函数 QApplication 前 加入 QT_HIGH_DPI()
		#if ((QT_VERSION >= QT_VERSION_CHECK(5,6,0)) && (QT_VERSION < QT_VERSION_CHECK(6,0,0))) 
			#define QT_HIGH_DPI() \
				QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); \
				QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); \
				_QT_HIGH_DPI_ROUNDING_POLICY();
		#else // QT_VERSION
			#define QT_HIGH_DPI()
		#endif // QT_VERSION
		
		
		
		//控件不能获取焦点
		#define SET_NO_FOCUS() \
				setFocusPolicy((Qt::FocusPolicy::NoFocus));
		
		
		
		
	#endif // QT_VERSION
	
	#if ((__cplusplus < 201103L) && ((defined __cplusplus) && (_MSVC_LANG < 201103L)))
		#include <type_traits>
		
		template <typename P>
		void DeleteAndSetNull(P& p){
			static_assert(std::is_pointer<P>::value, "Parameter must be a pointer type.");
			delete p;
			p = nullptr;
		}
		
		template <typename P>
		void DeleteAndSetNull_Array(P& p){
			static_assert(std::is_pointer<P>::value, "Parameter must be a pointer type.");
			delete[] p;
			p = nullptr;
		}
	#endif // __cplusplus < 201103L


#endif // __cplusplus



#ifdef _DEBUG
	
	
	
#endif //_DEBUG



//* 中文编程试验
//#define ； ;
//#define 如果 if
//#define 顺序循环 for
//#define 空 void
//#define 整数型 int
//#define 无符号整数型 uint32_t
//#define 终止运行 abort
//#define 输出 printf
//#define 返回 return
//#define 使用 using
//#define 布尔型 bool
//#define 真 true
//#define 假 false
//#define 类 class
//#define 结构体 struct
//#define 浮点型 float
//#define 双精度浮点型 double
//#define 核函数 __global__
//#define 显存分配 cudaMalloc
//#define 显存拷贝 cudaMemcpy




#endif //TERMINAL_BEAUTY_H__
