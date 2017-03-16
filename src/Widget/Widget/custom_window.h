#ifndef CUSTOM_WINDOW_H
#define CUSTOM_WINDOW_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class CustomWindow : public QDialog
{
	Q_OBJECT

public:
	CustomWindow(QWidget *parent);
	~CustomWindow();

	// 设置界面标题与图标
	virtual bool eventFilter(QObject *obj, QEvent *event);

protected:
	QVBoxLayout *m_pLayout;

private:
	QLabel *m_pIconLabel;
	QLabel *m_pTitleLabel;
	QPushButton *m_pCloseButton;
};

#endif // CUSTOM_WINDOW_H
