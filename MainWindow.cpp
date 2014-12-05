#include <QtGui>
#include <MainWindow.h>
#include <Dialog.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

MainWindow::MainWindow()
{
	createAction();
	createMenu();
	statusBar();
	setFixedSize(820, 660);
	setWindowTitle(tr("Nearest Point Pair"));
	stack = new QPoint [STACK_TOP];
	tstack = new int [STACK_TOP];
	top = 0;
	ttop = 0;
	tx = ty = 0;
	dialog = new Dialog;
	connect(dialog, SIGNAL(randomPoints(const int &)), this, SLOT(randomPoints(const int &)));
	clock();
}

MainWindow::~MainWindow()
{
	deleteAction();
	deleteMenu();
	delete [] stack;
	delete [] tstack;
	delete dialog;
}

void MainWindow::createAction()
{
	clearAction = new QAction(tr("&Clear"), this);
	clearAction->setStatusTip(tr("Clear all points."));
	connect(clearAction, SIGNAL(triggered()), this, SLOT(clearPoints()));
	randomAction = new QAction(tr("&Random"), this);
	randomAction->setStatusTip(tr("Use random points."));
	connect(randomAction, SIGNAL(triggered()), this, SLOT(showDialog()));
	exitAction = new QAction(tr("&Exit"), this);
	exitAction->setStatusTip(tr("Exit the software."));
	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createMenu()
{
	startMenu = menuBar()->addMenu(tr("&Start"));
	startMenu->addAction(clearAction);
	startMenu->addAction(randomAction);
	startMenu->addAction(exitAction);
}

void MainWindow::deleteAction()
{
	delete clearAction;
	delete randomAction;
	delete exitAction;
}

void MainWindow::deleteMenu()
{
	delete startMenu;
}

double getDist(const QPoint &x, const QPoint &y)
{
	if ((x.x() == y.x()) && (x.y() == y.y()))
		return 1e50;
	double ans = x.x() - y.x();
	ans *= (x.x() - y.x());
	double tans = x.y() - y.y();
	tans *= (x.y() - y.y());
	ans += tans;
	return ans;
}

void MainWindow::randomPoints(const int &x)
{
	if ((x > STACK_TOP) || (x < 0))
	{
		QMessageBox::warning(this, tr("Nearest Point Pair"), tr("The number should be between 0 to 1000000."));
		return;
	}

	srand(time(NULL));
	top = x;
	if (top <= 200)
	{
		for (int i = 0; i < top; i++)
		{
			stack[i].setX(rand() % 790 + 15);
			stack[i].setY(rand() % 590 + 35);
		}
	}
	else
	{
		for (int i = 0; i < top; i++)
		{
			stack[i].setX(rand());
			stack[i].setY(rand());
			if (RAND_MAX == SHRT_MAX)
			{
				stack[i].setX((stack[i].x() << 16) + rand());
				stack[i].setY((stack[i].y() << 16) + rand());
			}
		}
	}

	if (top > 1)
		getNearest_Quick();
	update();
}

void MainWindow::clearPoints()
{
	top = 0;
	update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	QImage image(size(), QImage::Format_ARGB32_Premultiplied);
	QPainter imagePainter(&image);
	imagePainter.initFrom(this);
	imagePainter.setRenderHint(QPainter::Antialiasing, true);
	imagePainter.eraseRect(rect());
	draw(&imagePainter);
	imagePainter.end();

	QPainter windowPainter(this);
	windowPainter.drawImage(0, 0, image);
}

void MainWindow::draw(QPainter *painter)
{
	painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap));
	painter->setBrush(QBrush(Qt::white, Qt::SolidPattern));
	painter->drawRect(10, 30, 800, 600);

	if (top > 200)
	{
		painter->drawText(20, 250, tr("The nearest point pair is: (%1,%2) and (%3,%4).").arg(stack[tx].x()).arg(stack[tx].y()).arg(stack[ty].x()).arg(stack[ty].y()));
		painter->drawText(20, 280, tr("Their distance is %1.").arg(sqrt(getDist(stack[tx], stack[ty]))));
		painter->drawText(20, 310, tr("The time cost of normal algorithm: %1 ms.").arg(time_Normal));
		painter->drawText(20, 340, tr("The time cost of quick algorithm: %1 ms.").arg(time_Quick));
		return;
	}

	painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
	for (int i = 0; i < top; i++)
		painter->drawEllipse(stack[i], SIZE_POINT, SIZE_POINT);
	painter->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
	if (top > 1)
	{
		painter->drawEllipse(stack[tx], SIZE_POINT, SIZE_POINT);
		painter->drawEllipse(stack[ty], SIZE_POINT, SIZE_POINT);
	}
}

void MainWindow::getNearest_Normal()
{
	double min = 1e50;
	double tmin;

	for (int i = 0; i < top; i++)
		for (int j = i + 1; j < top; j++)
		{
			tmin = getDist(stack[i], stack[j]);
			if (tmin < min)
			{
				min = tmin;
				tx = i;
				ty = j;
			}
		}
}

void MainWindow::getNearest_Quick()
{
	time_t t;
	t = clock();
	getNearest_Normal();
	time_Normal = clock() - t;
	t = clock();
	qsortPoints(0, top - 1);
	findNearestPoint(0, top - 1, tx, ty);
	time_Quick = clock() - t;
	time_Quick = time_Quick * 1000 / CLOCKS_PER_SEC;
	time_Normal = time_Normal * 1000 / CLOCKS_PER_SEC;
}

void MainWindow::qsortPoints(const int &x, const int &y)
{
	int i = x, j = y;
	QPoint k = stack[(x + y) >> 1], t;

	while (i < j)
	{
		while (stack[i].x() < k.x())
			i++;
		while (stack[j].x() > k.x())
			j--;
		if (i <= j)
		{
			t = stack[i];
			stack[i] = stack[j];
			stack[j] = t;
			i++;
			j--;
		}
	}

	if (x < j)
		qsortPoints(x, j);
	if (y > i)
		qsortPoints(i, y);
}

void MainWindow::qsortTstack(const int &x, const int &y)
{
	int i = x, j = y, k = tstack[(x + y) >> 1], t;

	while (i < j)
	{
		while (stack[tstack[i]].y() < stack[k].y())
			i++;
		while (stack[tstack[j]].y() > stack[k].y())
			j--;
		if (i <= j)
		{
			t = tstack[i];
			tstack[i] = tstack[j];
			tstack[j] = t;
			i++;
			j--;
		}
	}

	if (x < j)
		qsortTstack(x, j);
	if (y > i)
		qsortTstack(i, y);
}

double MainWindow::findNearestPoint(const int &left, const int &right, int &x, int &y)
{
	if (left == right)
		return 1e50;
	if ((left + 1) == right)
	{
		x = left;
		y = right;
		return getDist(stack[x], stack[y]);
	}
	
	double ans, tans;
	int ttx, tty, mid = (left + right) >> 1;
	ans = findNearestPoint(left, mid, x, y);
	tans = findNearestPoint(mid, right, ttx, tty);
	if (tans < ans)
	{
		x = ttx;
		y = tty;
		ans = tans;
	}
	ttop = 0;
	for (int i = 0; ; i++)
	{
		if ((mid - i) < left)
			break;
		if (stack[mid - i].x() + ans <= stack[mid].x())
			break;
		tstack[ttop] = mid - i;
		ttop++;
	}
	for (int i = 1; ; i++)
	{
		if ((mid + i) > right)
			break;
		if ((stack[mid].x() + ans) <= stack[mid + i].x())
			break;
		tstack[ttop] = mid + i;
		ttop++;
	}
	qsortTstack(0, ttop - 1);
	for (int i = 0; i < ttop; i++)
		for (int j = 1; j < 8; j++)
		{
			if ((i + j) >= ttop)
				break;
			tans = getDist(stack[tstack[i]], stack[tstack[i + j]]);
			if (tans < ans)
			{
				ans = tans;
				x = tstack[i];
				y = tstack[i + j];
			}
		}
	return ans;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (top >= 200)
			return;
		if ((event->x() >= 15) && (event->x() <= 805) && (event->y() >= 35) && (event->y() <= 625))
		{
			stack[top].setX(event->x());
			stack[top].setY(event->y());
			top++;
		}
		getNearest_Quick();
		update();
	}
}

void MainWindow::showDialog()
{
	dialog->show();
	dialog->raise();
	dialog->activateWindow();
}

