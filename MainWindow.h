#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include <QMainWindow>
#define STACK_TOP 1000000
#define SIZE_POINT 3

class QMenu;
class QAction;
class QPoint;
class Dialog;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();
protected slots:
	void clearPoints();
	void randomPoints(const int &x);
	void showDialog();
protected:
	void createAction();
	void createMenu();
	void deleteAction();
	void deleteMenu();
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void draw(QPainter *painter);
	void getNearest_Normal();
	void getNearest_Quick();
	void qsortPoints(const int &x, const int &y);
	double findNearestPoint(const int &left, const int &right, int &x, int &y);
	void qsortTstack(const int &x, const int &y);
private:
	QMenu *startMenu;
	QAction *clearAction;
	QAction *randomAction;
	QAction *exitAction;
	Dialog *dialog;
	QPoint *stack;
	int *tstack;
	int top ,ttop;
	int tx, ty;
	time_t time_Normal, time_Quick;
};

#endif

