CXXFLAGS=-Wall -Wextra -std=c++14 -O3 -fPIC
INCLUDES=-I/usr/include/qt/ -I/usr/include/qt/QtCore -I/usr/include/qt/QtGui -I/usr/include/qt/QtWidgets -I/usr/include/qtermwidget5/
LDFLAGS=-lQt5Core -lQt5Gui -lQt5Widgets -lqtermwidget5

all:
	mkdir -p bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) src/main.cpp -o bin/zdict.qt
