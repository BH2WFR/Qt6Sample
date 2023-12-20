#include "main.h"
#include "mainwindow.h"

#include <qglobal.h>
#include <QObject>
#include <QApplication>
#include <TerminalBeauty.h>
#include <QTimer>

#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>

#include <fmt/core.h>
#include <fmt/format.h>


class Test{
public:
	int num {0};
	Test() {
		fmt::println("构造 Test");
	}
	Test(int num) {
		this->num = num;
		fmt::println("构造 Test, num={}", num);
	}
	~Test() {
		fmt::println("析构 Test, num={}", num);
	}
};

QTimer* gp_timer;

void A::myTimerSlot1()
{
	static int num {0};
	fmt::println("timer slot, num = {}", num);
	Sleep(200);
	gp_timer->setInterval(gp_timer->interval() - 10);
	fmt::println("{}", gp_timer->interval());
	
}


int main(int argc, char *argv[])
{
	
	CONSOLE_TITLE("Qt6Test");
	CONSOLE_UTF_8();
	QT5_HIGH_DPI();
	PRINT_ARGV(argc, argv);
	PRINT_ERR("错误信息");
	//RETURN_IF_CUDA_ERR(1, "shabi", 0);
	//Test test;
	QApplication app(argc, argv);
	MainWindow w;
	
	//* 调试命令行
	QDEBUGNN << CQFlash CInverse CBold "============ Project Debugging  =============" CDefault;
	
	A aa;
	aa.MySignal();
	
	gp_timer = new QTimer(nullptr); // 参数改为 父类 Q_Object 的 指针
	QObject::connect(gp_timer, &QTimer::timeout, &aa, &A::myTimerSlot1);
	gp_timer->setSingleShot(true);
	gp_timer->start(1000); //每秒触发一次
	
	
	
	std::unique_ptr<A> b1 {std::make_unique<B1>()};
	emit b1->MySignal();
	
	std::string a {};

	auto metaObject = b1->metaObject(); //获取 metaobject, 在读取名字索引的时候会用到█
	if (!metaObject) // Qt 中每一个类(不论继承关系) 都会有 metaObject, 这样做不会读取父类的 MEtaobject
		return -1;
	
	// 获取属性组并转化为 QMap
	auto propertyGroupVariant= b1->property("propertyGroup");
	if (!propertyGroupVariant.isValid()) {
		return -1;
	}
	if (!propertyGroupVariant.canConvert<QVariantMap>()) {
		return -1;
	}
	auto propertyGroup = propertyGroupVariant.toMap();
	// 
	
	//* 读取 属性组
	for (auto iter = propertyGroup.keyValueBegin(); iter != propertyGroup.keyValueEnd(); iter++) {
		auto name =  iter->first; //获取属性名称
			auto jsonObject = iter->second.toJsonObject(); //qvariant 转换成 qjson
			if (jsonObject.count() == 0) {//json 是空的, 此时表示一个组
				//TODO：　分组适配
				// auto propertyItem = groupManager->addProperty(name);
				// addGroupProperty(propertyItem, name, jsonObject);
				
				
			}else{
				//获取属性索引和属性元信息
				auto index = metaObject->indexOfProperty(name.toStdString().c_str());
				if (index == -1) {
					continue;
				}
				QMetaProperty metaProperty = metaObject->property(index);
				//内含很多类型判断函数
/*
	bool isReadable() const;
	bool isWritable() const;
	bool isConstant() const;
	QVariant read(const QObject *obj) const;
	bool write(QObject *obj, const QVariant &value) const;
	bool reset(QObject *obj) const;
*/

				int type = metaProperty.userType(); //拿到类型
				switch (type){
					case QMetaType::Int:
							fmt::println("{} : {} = {}", name.toStdString(), type, metaProperty.read(b1.get()).toInt());
						break;
					case QMetaType::Double:
							fmt::println("{} : {} = {}", name.toStdString(), type, metaProperty.read(b1.get()).toDouble());
						break;
					// case 666:
						
					//     break;
					default:
						
						break;
				}
				
			}
	}
	
	//fmt::println("b1.b1pri1 = {}", b1->property("b1pri1"));
	
	std::shared_ptr<A> b2 {std::make_shared<B2>()};
	emit b2->MySignal();
	
	w.show();
	return app.exec();
}

QMap<QString, QVariant> B1::propertyGroup()
{
	QMap<QString, QVariant> testmap;
	QJsonObject json;
	
	//顺序无关
	
	json = {
		{"name", "b1pri2"},
		{"min", 0},
		{"max", 20},
		
	}; //可以设置更多的信息, 比起按照序号访问的反射
	testmap.insert("b1pri2", json); //QJson 包裹到 QVariant 中
	
	json = {
		{"name", "b1pri1"},
		{"min", 0},
		{"max", 10},
		
	}; //可以设置更多的信息, 比起按照序号访问的反射
	testmap.insert("b1pri1", json); //QJson->QVariant
	
	return testmap;
}
