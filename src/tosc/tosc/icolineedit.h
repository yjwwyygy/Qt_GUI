#ifndef ICOLINEEDIT_H
#define ICOLINEEDIT_H

#include <QLineEdit>
#include <QToolButton>

class IcoLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	IcoLineEdit(QWidget *parent = nullptr);
	~IcoLineEdit();

public:
	void setFavIcon(const QIcon &icon);

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	QToolButton *m_favButton;	
};

#endif // ICOLINEEDIT_H
