#ifndef TABBUTTON_H
#define TABBUTTON_H

#include <QToolButton>

class TabButton : public QToolButton
{
	Q_OBJECT

public:
	TabButton(QWidget *parent = nullptr);
	~TabButton();

	Q_PROPERTY(int m_id READ getId WRITE setId)
	int getId();
	void setId(int id);

signals:
	void tabCloseClicked();

protected:
	void resizeEvent(QResizeEvent *event) override;

private:
	int m_id;
	QToolButton *m_clearButton;
};

#endif // HTOOLBUTTON_H
