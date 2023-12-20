cmake_minimum_required(VERSION 3.20 FATAL_ERROR) 			#== 最低版本
project(Qt6Sample VERSION 0.0.2 LANGUAGES CXX)	#== 指定工程名和编程语言 
#会产生 CMAKE_PROJECT_NAME: 顶层工程名称  PROJECT_NAME 当前工程名称 PROJECT_SOURCE_DIR 当前工程的源码路径

# ===== ========== 移植指南 ==================



#================= 重点变量和全局宏定义: ==============


#* 添加宏定义
#target_compile_definitions(MyLib PRIVATE MY_MACRO) #当前路径下
#add_compile_definitions(MYDEFINITION)

#== 提前设置 target 名
set(PROJ_EXE_TargetName_1 Qt6Test) #== 设置 target 名, cmake 中 target 是指一个生成的可执行文件或库的名字


#== 目录设置 =========
#* 不要取消注释, 这几个目录已经在 CMakePresets.json 里面设置好了
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMakeTargets") # 把 ALL_BUILD 和 ZERO_CHECK 放到 CMakeTargets 文件夹下
set(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/bin)
set(CMAKE_PREFIX_PATH "")

# set(LIBRARY_OUTPUT_PATH    "${CMAKE_BINARY_DIR}/OUTPUT") #* 生成的库和可执行文件放到哪里?
#set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/OUTPUT/EXECUTABLE") # 不这样设置可能会导致复杂工程中 exe 找不到 dll

set(PROJ_OUTPUT_DIR "${CMAKE_BINARY_DIR}/OUTPUT")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJ_OUTPUT_DIR}/lib") # 静态库lib, windows动态库导出符号lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJ_OUTPUT_DIR}/lib") # 动态库so/dylib, Windows MODULE动态库dll
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJ_OUTPUT_DIR}/bin") # 可执行文件, Windows SHARED 动态库dll

# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG 	${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE  ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG 	${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE 	${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG 	${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE 	${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


#* 以上几个路径被
# ARCHIVE_OUTPUT_DIRECTORY  .lib				(前面加 CMAKE_ 代表顶层的)
# LIBRARY_OUTPUT_DIRECTORY  .dll(module)
# RUNTIME_OUTPUT_DIRECTORY  .exe .dll(shared)
# 取代！


#==设置install输出目录 (本工程只在release构建下才有install, 因此不区分debug和release)
SET(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/INSTALL) #默认 C:\program files, 这里使用 .\install 目录



#====================== 编译配置 ==================   
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #用于 clangd 导出

set(CMAKE_CXX_STANDARD 20) # C++版本
set(CMAKE_C_STANDARD   17) # C17, 
set(CMAKE_CXX_STANDARD_REQUIRED ON) #不支持指定 C++ 标准就报错
set(CMAKE_CXX_EXTENSIONS OFF) #是否使用GCC等扩展特性

add_compile_definitions(UNICODE _UNICODE )  #开启unicode宏, 使用unicode字符编程
add_definitions(-D MYLIBDLL_EXPORT)   #一个不知道干啥的宏定义, 大概作用是编译生成的dll库函数允许外部调用?


set(CMAKE_DEBUG_POSTFIX "d") #* 输出自己写的 dll 库, 给 Debug 模式下自动添加后缀 "d"
#option(USE_SOLUTION_FOLDERS  ON)


if(WIN32) #* windows 下, 从控制台启动还是从窗口启动? 
	if(CMAKE_BUILD_TYPE STREQUAL "Debug") # .==== debug
		set(CMAKE_WIN32_EXECUTABLE OFF) # windows下, ON 生成没有控制台窗口的 win32 程序, OFF 则生成控制台程序, 由控制台调出 Qt 窗口
			# MSVC 参数: /SUBSYSTEM:CONSOLE,  若要使用 qDebug(), 请设置为 OFF
	else() # .===== release
		set(CMAKE_WIN32_EXECUTABLE ON) # 不使用控制台, 直接启动 GUI
			# MSVC 参数: /SUBSYSTEM:WINDOWS
	endif()
endif()

#==== ↓↓ 各编译器参数设置  ↓↓ =======
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")  #* 如果在 MSVC 编译器上

	#* == 默认启动项目设置
	set_property ( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJ_EXE_TargetName_1} )	
	#message("flags: ${CMAKE_CXX_FLAGS}")
	
	#* == 编译器参数设置
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")	#开启多核编译
	set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /MP")
	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8") # 默认文件编码设置为 UTF-8, 否则 MSVC 会默认按照 GB2312 编码
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /sdl-") # 关闭 SDL 检查, 允许使用 sscanf() 之类的不安全函数
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus") #让__cplusplus 宏等于当前最新的c++版本, 不支持c++11, 仅限14,17,20, 版本>VS2017有效
	
	#* 清除 debug/release 专用参数
	set(CMAKE_CXX_FLAGS_DEBUG   "/Zi /RTC1")
	set(CMAKE_CXX_FLAGS_RELEASE "/DNDEBUG")
	
	
	#* == 选择集成的 MSVC 运行时库
	set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} /MDd") #MD MT MDd MTd
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
	# https://cmake.org/cmake/help/latest/prop_tgt/MSVC_RUNTIME_LIBRARY.html
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL") #* 不加这个可能不生效
	else() # release
		set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
		# 注意: 如果 Release 使用静态 Qt 库, 编译时若使用 "/MD", 可能报几百个错误
	endif()
	
	#* == 代码优化
	set(CMAKE_CXX_FLAGS_DEBUG  	"${CMAKE_CXX_FLAGS_DEBUG} /Od /Ob0") 	# debug 关闭优化
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /Ob2") 	# release 开优化
	
	
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")  #* 如果在 GCC 编译器上
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
	message(FATAL_ERROR "--------- temporary only supports MSVC compiler and Windows NT OS...")

else()
	message(FATAL_ERROR "--------- temporary only supports MSVC compiler and Windows NT OS...")
endif() #MSVC/GCC/CLANG



#======================= 函数 ===================

#* ↓↓↓ 输出信息
macro(Proj_SHOW_MSG)
	message(STATUS "\n\n\n")
	message(STATUS "=======================  输出配置: ${PROJECT_NAME} =========================")
	message(STATUS "CMAKE_CURRENT_LIST_LINE: ${CMAKE_CURRENT_LIST_LINE}") #当前行行号
	message(STATUS "工程名称: ${PROJECT_NAME}")
	message(STATUS "工程版本: ${PROJECT_VERSION}")
	message(STATUS "CMake 版本:		${CMAKE_VERSION}")
	message(STATUS "构建工具:		${CMAKE_GENERATOR}")
	message(STATUS "\n" )
	
	message(STATUS "C++编译器:		${CMAKE_CXX_COMPILER_ID}  ${CMAKE_CXX_COMPILER_VERSION}") #${CMAKE_CXX_COMPILER_ID}
	message(STATUS "C 编译器:		${CMAKE_C_COMPILER_ID}  ${CMAKE_C_COMPILER_VERSION}")
	message(STATUS "CUDA 编译器:   	${CMAKE_CUDA_COMPILER_ID}  ${CMAKE_CUDA_COMPILER_VERSION}")
	message(STATUS "\n" )

	
	message(STATUS "CMAKE_CONFIGURATION_TYPES: 	${CMAKE_CONFIGURATION_TYPES}")
	message(STATUS "CMAKE_BUILD_TYPE: 			${CMAKE_BUILD_TYPE}") #build还是release?
	message(STATUS "\n" )
	
	message(STATUS "PROJECT_SOURCE_DIR:		${PROJECT_SOURCE_DIR}") #* 当前工程 CMake 文件所在目录
	message(STATUS "PROJECT_BINARY_DIR:		${PROJECT_BINARY_DIR}") #* 当前工程 build 目录
	message(STATUS "CMAKE_SOURCE_DIR:		${CMAKE_SOURCE_DIR}") #* 顶层 CMake 文件所在目录(整个项目根目录)
	message(STATUS "CMAKE_BINARY_DIR:		${CMAKE_BINARY_DIR}") #* 顶层 CMake 的 build 目录
	message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}") # 正在处理的项目的 
	message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}") #* 当前 cmake 的 build 目录
	message(STATUS "CMAKE_CURRENT_LIST_DIR:	${CMAKE_CURRENT_LIST_DIR}") #* 当前 cmake 文件路径
	message(STATUS "CMAKE_MODULE_PATH:		${CMAKE_MODULE_PATH}" ) #CMake 模块的路径列表
	message(STATUS "\n" )

	message(STATUS "EXECUTABLE_OUTPUT_PATH: ${EXECUTABLE_OUTPUT_PATH}" ) #deprecated! 
	message(STATUS "LIBRARY_OUTPUT_PATH:    ${LIBRARY_OUTPUT_PATH}" )
	message(STATUS "\n" )
	
	message(STATUS "CMAKE_ARCHIVE_OUTPUT_DIRECTORY:		${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")# 静态库lib  动态库符号lib(dll)
	message(STATUS "CMAKE_LIBRARY_OUTPUT_DIRECTORY:		${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")# MODULE库dll, 动态库so/dylib (windows 下不输出)
	message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY:		${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")# 可执行exe SHARED动态库dll
	message(STATUS "CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY:	${CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY}")
	message(STATUS "CMAKE_PDB_OUTPUT_DIRECTORY:			${CMAKE_PDB_OUTPUT_DIRECTORY}")
	message(STATUS "\n" )
	message("flags: ${CMAKE_CXX_FLAGS}")
	message("flags_debug: ${CMAKE_CXX_FLAGS_DEBUG}")
	message("flags_release: ${CMAKE_CXX_FLAGS_RELEASE}")
	
	message(STATUS "===========  ${PROJECT_NAME}: install 配置 ==============")
	message(STATUS "Install Destination: 	${CMAKE_INSTALL_PREFIX}") 
	message(STATUS "Install LibDir:			${CMAKE_INSTALL_LIBDIR}") 
	message(STATUS "==============  ${PROJECT_NAME}: 打印完成      =================\n\n" )
endmacro()
#set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")



#====================== 引入第三方库 ===================
#* 第三方库文件大礼包位置
get_filename_component(3rdParty_DIR "D:/Projects/3rdlibs" ABSOLUTE) #* 展开相对路径为绝对路径
message("\n\n3rdParty_DIR: ${3rdParty_DIR}\n\n")


#=== ============ Qt ===================
message(STATUS "\n\n============== Qt6 library status:=====================")

set(CMAKE_AUTOUIC OFF) # Qt 中 自动翻译界面文件为cpp文件, 暂时关闭手动执行, 因为不指定 CMAKE_AUTOUIC_SEARCH_PATHS 会抽风, QT6_WRAP_UI()
set(CMAKE_AUTOMOC ON)  # Qt 中 自动翻译信号与槽
set(CMAKE_AUTORCC ON)  # Qt 中 自动翻译资源文件为cpp文件
set(CMAKE_INCLUDE_CURRENT_DIR ON) # 同时包含进来 build 目录的代码文件(Qt的moc机制会把moc文件生成到生成目录里面)

#* 设置 Qt 动态库和静态库的目录
set(PROJ_Qt6_MSVC_x64_PATH "C:/Program_Files/Qt/6.5.2/msvc2019_64")
set(PROJ_Qt_PATH ${PROJ_Qt6_MSVC_x64_PATH})  		#* 引入动态的Qt库


# 设置 Qt 的 cmake 目录让系统能搜索到
set(PROJ_Qt_CMAKE_PATH ${PROJ_Qt_PATH}/lib/cmake)
#set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${PROJ_Qt_CMAKE_PATH}) #关键代码
#qt_standard_project_setup()

#* 寻找 Qt 包
find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED PATHS ${PROJ_Qt_CMAKE_PATH})
	# 后续添加 Qt 模块! 勿删此注释


message(STATUS "Qt6_DIR 	${Qt6_DIR}")
message(STATUS "Qt6Core_DIR ${Qt6Core_DIR}")
message(STATUS "Qt6Gui_DIR 	${Qt6Gui_DIR}")
message(STATUS "Qt6Widgets_DIR 		${Qt6Widgets_DIR}")
message(STATUS "PROJ_Qt_CMAKE_PATH 	${PROJ_Qt_CMAKE_PATH}")
message("\n\n")



#=== ================== FMT =====================
set(FMT_DIR "${3rdParty_DIR}/fmt-10.1.1")
find_package(fmt REQUIRED  PATHS ${FMT_DIR}/lib/cmake/fmt)
# 在 target 的 target_link_libraries 中手动添加 ${FMT_LIBS}


#=== =========== TerminalBeauty =====================
#include_directories(AFTER "${3rdParty_DIR}/GSB-TerminalBeauty")
include_directories(AFTER "${CMAKE_SOURCE_DIR}/GSB-TerminalBeauty")


#== ================ 自创库 =================




#======================= 输出信息 =====================
Proj_SHOW_MSG()



#=========== 安装 (install) 时拷贝第三方库 dll 文件到安装路径 ==============

if(CMAKE_BUILD_TYPE STREQUAL "Release") #== install 仅作用于 release模式下
	message(STATUS "Release 模式: 正在将项目所需 dll 文件拷贝到 install 目录下...")	
	
	
	#* ↓↓↓ (可选) 先删除 install目录下所有文件
	file(REMOVE_RECURSE ${CMAKE_INSTALL_PREFIX})
	
	
	#== 将 外部 DLL 库 输出到指定目录
	#* Qt6
	if(NOT PROJ_Qt_STATIC) # release && 使用静态库 才会触发这个变量
		message(STATUS "PROJ_Qt: Configuring Qt6 Libraries Installation...")
		if(WIN32) #* windows系统下
			install(
			  FILES
				${PROJ_Qt_PATH}/bin/Qt6Core.dll
				${PROJ_Qt_PATH}/bin/Qt6Gui.dll
				${PROJ_Qt_PATH}/bin/Qt6Widgets.dll
			  DESTINATION ${CMAKE_INSTALL_PREFIX}
			)
			install(
				FILES ${PROJ_Qt_PATH}/plugins/platforms/qwindows.dll 
				DESTINATION ${CMAKE_INSTALL_PREFIX}/platforms
			)
			install(
				FILES ${PROJ_Qt_PATH}/plugins/styles/qwindowsvistastyle.dll 
				DESTINATION ${CMAKE_INSTALL_PREFIX}/styles
			)
			#== 后续添加 Qt 模块!
			
		else()	#linux系统下
			
		endif()
			
	else()
		message(STATUS "PROJ_Qt: Using Static Qt Library, No Need to Install")
	endif()
	
	
	
	
else() #* debug模式
	message("WARNING: Installation is Only Available in Release Mode, so There's no Install Project in Debug Mode!") #不进行install操作
endif()


message(STATUS "\n\n===== ${PROJECT_NAME}: CONFIGURATION COMPLETE =====\n\n")




#==================  添加子目录 ====================
add_subdirectory(Qt6Test)
