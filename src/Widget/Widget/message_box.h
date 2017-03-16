#ifndef MESSAGE_BOX_H
#define MESSAGE_BOX_H

#include "custom_window.h"
#include <QMessageBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>

class MessageBox : public CustomWindow
{
	Q_OBJECT

public:
	MessageBox(QWidget *parent = 0, const QString &title = tr("Tip"), const QString &text = "",
		QMessageBox::StandardButtons buttons = QMessageBox::Ok, QMessageBox::StandardButton defaultButton = QMessageBox::Ok);
	~MessageBox();

	QAbstractButton *clickedButton() const;
	QMessageBox::StandardButton standardButton(QAbstractButton *button) const;
	// 设置默认按钮
	void setDefaultButton(QPushButton *button);
	void setDefaultButton(QMessageBox::StandardButton button);
	// 设置窗体标题
	void setTitle(const QString &title);
	// 设置提示信息
	void setText(const QString &text);
	// 设置窗体图标
	void setIcon(const QString &icon);
	// 添加控件-替换提示信息所在的QLabel
	void addWidget(QWidget *pWidget);

protected:
	// 多语言翻译
	void changeEvent(QEvent *event);

	private slots:
	void onButtonClicked(QAbstractButton *button);

private:
	void translateUI();
	int execReturnCode(QAbstractButton *button);

private:
	QLabel *m_pIconLabel;
	QLabel *m_pLabel;
	QGridLayout *m_pGridLayout;
	QDialogButtonBox *m_pButtonBox;
	QAbstractButton *m_pClickedButton;
	QAbstractButton *m_pDefaultButton;
};

QMessageBox::StandardButton showInformation(QWidget *parent, const QString &title,
	const QString &text, QMessageBox::StandardButtons buttons,
	QMessageBox::StandardButton defaultButton);

QMessageBox::StandardButton showError(QWidget *parent, const QString &title,
	const QString &text, QMessageBox::StandardButtons buttons,
	QMessageBox::StandardButton defaultButton);

QMessageBox::StandardButton showSuccess(QWidget *parent, const QString &title,
	const QString &text, QMessageBox::StandardButtons buttons,
	QMessageBox::StandardButton defaultButton);

QMessageBox::StandardButton showQuestion(QWidget *parent, const QString &title,
	const QString &text, QMessageBox::StandardButtons buttons,
	QMessageBox::StandardButton defaultButton);

QMessageBox::StandardButton showWarning(QWidget *parent, const QString &title,
	const QString &text, QMessageBox::StandardButtons buttons,
	QMessageBox::StandardButton defaultButton);

QMessageBox::StandardButton showCritical(QWidget *parent, const QString &title,
	const QString &text, QMessageBox::StandardButtons buttons,
	QMessageBox::StandardButton defaultButton);

QMessageBox::StandardButton showCheckBoxQuestion(QWidget *parent, const QString &title,
	const QString &text, QMessageBox::StandardButtons buttons,
	QMessageBox::StandardButton defaultButton);

#endif // MESSAGE_BOX_H
