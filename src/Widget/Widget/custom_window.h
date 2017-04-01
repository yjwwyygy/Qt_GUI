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

	// ���ý��������ͼ��
	virtual bool eventFilter(QObject *obj, QEvent *event);

private slots:

	// ������С�������/��ԭ���رղ���
	void onClicked();

protected:
	QVBoxLayout *m_pLayout;

private:
	QLabel *m_pIconLabel;
	QLabel *m_pTitleLabel;
	QPushButton *m_pCloseButton;
};

#endif // CUSTOM_WINDOW_H
