﻿#ifndef TERMINAL_UTIL_H__
#define TERMINAL_UTIL_H__


#ifdef __cplusplus
	#include <cstdio>
	#include <cstdlib>
	#include <cstdint>
#else //. !__cplusplus
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdint.h>
	#include <stdbool.h>
#endif // __cplusplus


// include winapi headers
#if defined(_WIN32) || defined(WIN32) || defined(__WIN32)
	#include <windows.h>
#endif // WinAPI 适配


// #ifdef _MSC_VER
// 	#pragma warning( disable: 4003 4996 )
// #endif



//======================= 命令行增强宏定义 ==============================
//*参考 Ansi 转义序列:  https://zh.wikipedia.org/wiki/ANSI%E8%BD%AC%E4%B9%89%E5%BA%8F%E5%88%97
/*
使用说明:
	在 printf/std::cout/fmt::print 中, 字符串前后加入这些宏定义实现 字符颜色/光标位置 等操作:
	例:
		int main(int argc, char* argv[]){
			TERMINAL_UTF_8(); 			// 命令行编码改为 UTF_8
			TERMINAL_TITLE("MyProject");// 设置命令行窗口标题, 仅限 windows
			TERMINAL_SIZE(100, 30);		// 设置命令行窗口大小为 宽度 100 字符, 高度 30 行
			TERMINAL_CLEAR();			// 清空命令行
			
			PRINT_ARGV(argc, argv);		// 打印出程序的所有命令行参数
			
			printf(CLGreen "Hello World!\n" CDefault); // 使用明亮的黄色输出 "Hello World"
			CPRINTF(CLGreen, "Hello World!\n"); // 等价
			CNPRINTF(CLGreen, "Hello World!");  // 等价
			
			std::cout << CLRed "Error\n" CDefault; // 注意: "CDefault" 时命令行字符颜色恢复默认
			
			printf(BRed CGreen CQFlash "test\n" CDefault); // 红底绿色, 并快速闪烁
			
			printf(CRight(2)); // 光标上移两行
			
			PRINT_ERR("错误信息") // 打印一条错误信息, 并且附上文件名, 函数名, 和行数
			TERMINAL_PAUSE() // system("pause");
			return 0;
		}

*/
//* printf/std::cout 的颜色控制代码, 用于界面美化
// 字符颜色: 暗色系列
#define CBlack		"\033[30m"
#define CRed 		"\033[31m"
#define CGreen 		"\033[32m"
#define CBrown		"\033[33m"
#define CBlue		"\033[34m"
#define CPurple		"\033[35m"
#define CCyan		"\033[36m"
#define CWhite		"\033[37m"

// 字符颜色: 自定义 RGB
#define CFrontColor(C)		"\033[38;5;"#C"m"
#define CFrontRGB(R, G, B)	"\033[38;2;" #R ";" #G ";" #B "m"

//* 字符颜色: 亮色系列
#define CLBlack		"\033[90m"
#define CLRed 		"\033[91m"
#define CLGreen 	"\033[92m"
#define CLYellow 	"\033[93m"
#define CLBlue 		"\033[94m"
#define CLPurple 	"\033[95m"
#define CLCyan 		"\033[96m"
#define CLWhite 	"\033[97m"

// 字符背景颜色 (未完成)
#define BBlack 		"\033[40m"
#define BRed 		"\033[41m"
#define BWhite 		"\033[47m"
//TODO: 完善背景颜色

// 字符背景颜色: 自定义 RGB
#define CBackColor(C)		"\033[48;5;"#C"m"
#define CBackRGB(R, G, B)	"\033[38;2;" #R ";" #G ";" #B "m"

// 特殊颜色组合
#define CTurn 		"\033[107;30m"	// 白底黑字

//* 字符格式: 恢复默认, 需要在字符串后加入
#define CDefault 	"\033[0m"

// 字符状态
#define CBold		"\033[1m"	// 加粗
#define CWeak		"\033[2m"	// 变暗
#define CItalic		"\033[3m"	// 斜体
#define CUnderLine	"\033[4m"	// 下划线
#define CFlash		"\033[5m"	// 闪烁
#define CQFlash		"\033[6m"	// 快速闪烁
#define CInverse	"\033[7m"	//


//* 命令行窗口 光标控制
#define CUp(n)			"\033["#n"A" // 光标上移
#define CDown(n)		"\033["#n"B" // std::cout << CDown("2") 光标下移两行
#define CLeft(n)		"\033["#n"C" // 光标左移
#define CRight(n)		"\033["#n"D" // 光标右移
#define CNextLn(n)		"\033["#n"E"
#define CPrevLn(n)		"\033["#n"F"
#define CHorzPos(n)		"\033["#n"G"
#define CPos(x, y)		"\033["#x";"#y"H" // 设置光标位置
#define CScrollUp(n)	"\033["#n"S"
#define CScrollDn(n)	"\033["#n"T"
#define CSaveCursurPos	"\033[s"
#define CReadCursurPos  "\033[u"

// 命令行窗口 清除内容
#define CEraseAfter		"\033[0J"
#define CEraseBefore	"\033[1J"
#define CEraseAll		"\033[2J" 	// 清屏 但光标不动
#define CClearAll		"\033[3J" 	// 清屏+删除缓存区
#define CEraseLine(n)	"\033["#n"K"



//* 命令行窗口 和 输入流 相关宏
#define TERMINAL_UTF_8() 		system("chcp 65001");  //* 命令行用 utf-8
#define TERMINAL_CLEAR()		system("cls"); 		   // 清屏

#define TERMINAL_SIZE(X, Y) 	system("mode con cols=" #X "lines=" #Y); // 设置命令行窗口大小

#define TERMINAL_PAUSE()		system("pause");		// 暂停程序, 按任意键继续

// 吸收输入缓存区内的其余字符, 以便下次 scanf 或 cin 时能够获取到正确的输入内容
#define TERMINAL_ABSORB()		{char ch; while((ch = getchar()) != '\n') continue;}

//#if defined(_WIN32) || defined(WIN32) || defined(__WIN32)
#ifdef _WINDOWS_
	#define TERMINAL_TITLE(str)		SetConsoleTitleA(str);  // 设置命令行窗口标题, 仅限 windows
#else
	#define TERMINAL_TITLE(str)
#endif // WinAPI 适配


//* printf 函数增强懒人包
// 行尾附着换行符的 printf
#define NPRINTF(str, ...) 				printf(str "\n", __VA_ARGS__ );
// 可自定义颜色, 输出后恢复默认的 printf
#define CPRINTF(color, str, ... ) 		printf(color str CDefault, __VA_ARGS__ );
// 可自定义颜色, 输出后恢复默认, 行尾附着换行符的 printf
#define CNPRINTF(color, str, ... ) 		printf(color str CDefault "\n", __VA_ARGS__ );


//==================== 程序调试: 打印信息 与 错误处理 =========================
//* main 函数中依次打印出所有的 argc:argv 参数
#define PRINT_ARGV(argc, argv) { printf("\n" CLYellow); \
	printf(CLYellow CBold "====== Program params, all argc and argv[i] =====\n" CDefault);\
	printf(CLGreen "argc:   " CLWhite "%d\n", argc); \
	printf(CLGreen "argv[  0]: " CLBlue "%.256s\n", argv[0]); \
	for(int i = 1; i < argc; i++) printf(CLYellow "argv[%3d]: " CLCyan "%.256s\n", i, argv[i]); \
	printf(CDefault "\n"); }
		
// 打印错误信息, 会自动打印出 当前所在的文件, 行数, 和函数名
#define PRINT_ERR(str) \
	printf(CLRed "\n=============== ERROR: " CLYellow #str CLRed "\n\
    file: " __FILE__ "\n\
    func: %s\n\
    line: %d\n\n" CDefault , __func__, __LINE__);


// 强行退出程序
#define ABORT_ERR(str) {PRINT_ERR(str); exit(-1);}
#define ASMABORT_ERR(str) {PRINT_ERR(str); asm("exit;" );}

// 如果指针为空, 则强行退出程序
#define ABORT_NULLPTR(ptr, str) \
	{if(ptr == NULL) ABORT_ERR(str) }


//==================== CUDA: 部分懒人包 ==========================
#ifdef __CUDA_RUNTIME_H__ //* CUDA 错误检查与退出机制

// 进行 cuda 部分操作后, 将返回值传入此宏定义中, 如果执行失败, 则输出信息, 并退出当前函数, 第三个参数为 return 值, void 则不填写
#define CHK_CUDA_ERR(err, str, ...) \
	if (err != cudaSuccess) { \
		printf(CLRed "\n=============== CUDA ERROR %d: " #str "\n\
    file: " __FILE__ "\n\
    func: %s\n\
    line: %d\n" CDefault , err, __func__, __LINE__); \
		return ##__VA_ARGS__; \
	}

// 进行 cuda 部分操作后, 将返回值传入此宏定义中, 如果执行失败, 则输出信息并强行停止程序
#define ABORT_CUDA_ERR(err, str) \
	if (err != cudaSuccess) { \
		printf(CLRed "\n=============== CUDA ERROR %d: " #str "\n\
    file: " __FILE__ "\n\
    func: %s\n\
    line: %d\n" CDefault , err, __func__, __LINE__); \
		exit(-1); \
	}

#endif //__CUDA_RUNTIME_H__




//===================== Qt: 部分懒人包 ==========================
#if defined(QT_VERSION) && defined(__cplusplus)

#include <QDebug>

#ifdef QDEBUG_H //* qDebug() 懒人包
	#define QDEBUGNN qDebug().nospace().noquote() // 不输出空格和引号的 qDebug()
	#define QDEBUGN  qDebug().noquote() 		  // 不加引号的 qDebug()
#endif

//* QT5 高分屏支持, Qt [5.6.0, 6.x.x), 需要在 main 函数 QApplication 定义前 加入 QT_HIGH_DPI()
#if ((QT_VERSION >= QT_VERSION_CHECK(5,6,0)) && (QT_VERSION < QT_VERSION_CHECK(5,14,0)))
	#define QT5_HIGH_DPI() \
		QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); \
		QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#elif ((QT_VERSION >= QT_VERSION_CHECK(5,14,0)) && (QT_VERSION < QT_VERSION_CHECK(6,0,0)))
	#define QT5_HIGH_DPI() \
		QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling); \
		QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); \
		QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#else // QT_VERSION >=6.0.0 || <5.0.0
	#define QT5_HIGH_DPI()
#endif // QT_VERSION


// 使控件不能获得焦点
#define QT_SET_NO_FOCUS() \
		setFocusPolicy((Qt::FocusPolicy::NoFocus));


#endif // QT_VERSION


//===================== C++: 部分懒人包 ==========================
#ifdef __cplusplus

//* 检测 CPP 语言版本, 大于等于 或 小于, 参数如 "201103", 后面不加 "L"
#ifdef _MSVC_LANG
	#define CPP_VER_HIGHER_EQUAL_THAN(_VER) \
		( (defined __cplusplus) && (_MSVC_LANG >= _VER##L) )
	#define CPP_VER_LOWER_THAN(_VER) \
		( (defined __cplusplus) && (_MSVC_LANG <  _VER##L) )
#else //. !defined _MSVC_LANG
	#define CPP_VER_HIGHER_EQUAL_THAN(_VER) \
		(__cplusplus >= _VER##L)
	#define CPP_VER_LOWER_THAN(_VER) \
		(__cplusplus <  _VER##L)
#endif // _MSVC_LANG

#if CPP_VER_HIGHER_EQUAL_THAN(201103)
	#include <type_traits>
	
	//* delete 裸指针并置空
	template <typename P> void DeleteAndSetNull(P& p){
		static_assert(std::is_pointer<P>::value, "Parameter must be a pointer type.");
		delete p;
		p = nullptr;
	}
	
	//* delete[] 裸指针并置空
	template <typename P> void DeleteAndSetNull_Array(P& p){
		static_assert(std::is_pointer<P>::value, "Parameter must be a pointer type.");
		delete[] p;
		p = nullptr;
	}
#endif // __cplusplus < 201103L


#endif // __cplusplus



#ifdef _DEBUG
	
	
	
#endif //_DEBUG



#endif //TERMINAL_UTIL_H__
