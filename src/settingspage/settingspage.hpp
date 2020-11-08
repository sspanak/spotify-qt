#pragma once

#include "../settings.hpp"

#include <QVBoxLayout>
#include <QMessageBox>

class SettingsPage: public QWidget
{
Q_OBJECT

public:
	virtual QIcon icon() = 0;
	virtual QString title() = 0;

	virtual bool save() = 0;

protected:
	explicit SettingsPage(Settings &settings, QWidget *parent);

	void warning(const QString &title, const QString &message);
	void applyFail(const QString &setting);

	QVBoxLayout *layout = nullptr;
	Settings &settings;
};