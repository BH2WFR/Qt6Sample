#include "main.h"
#include "mainwindow.h"

#include <qglobal.h>
#include <QObject>
#include <QApplication>
#include <TerminalBeauty.h>

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <qmetaobject>
#include <QMetaProperty>
#include <qmetatype>


#include <fmt/core.h>
#include <fmt/format.h>




int main(int argc, char *argv[])
{
	QT5_HIGH_DPI();
	TERMINAL_TITLE("Qt6Test");
	PRINT_ARGV(argc, argv);
	
	QApplication a(argc, argv);
	MainWindow w;
	
	//* 调试命令行
	QDEBUGNN << CQFlash CLYellow "=================== Project Debugging  中文 ================" CDefault;
	
	A aa;
	aa.MySignal();
	

	
	std::unique_ptr<A> b1 {std::make_unique<B1>()};
	emit b1->MySignal();
	
	auto metaObject = b1->metaObject(); //获取 metaobject, 在读取名字索引的时候会用到
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
	return a.exec();
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
