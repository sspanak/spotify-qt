#include "logviewer.hpp"

LogViewer::LogViewer(QWidget *parent)
	: QDialog(parent)
{
	auto layout = new QVBoxLayout();
	setLayout(layout);
	resize(600, 400);
	setWindowTitle("Logs");

	auto list = new QTreeWidget(this);
	list->setHeaderLabels({
		"Time", "Type", "Message"
	});
	list->setEditTriggers(QAbstractItemView::NoEditTriggers);
	list->setSelectionBehavior(QAbstractItemView::SelectRows);
	list->setRootIsDecorated(false);
	list->setAllColumnsShowFocus(true);
	layout->addWidget(list, 1);

	auto buttons = new QDialogButtonBox();

	QPushButton::connect(buttons->addButton("Copy to clipboard", QDialogButtonBox::ActionRole),
		&QPushButton::clicked, this, &LogViewer::copyToClipboard);

	QPushButton::connect(buttons->addButton("Save...", QDialogButtonBox::ActionRole),
		&QPushButton::clicked, this, &LogViewer::saveToFile);

	layout->addWidget(buttons);

	for (auto &message : lib::log::get_messages())
	{
		auto item = new QTreeWidgetItem({
			QString::fromStdString(message.get_time()),
			QString::fromStdString(message.get_type()),
			QString::fromStdString(message.get_message())
		});

		list->addTopLevelItem(item);
	}
}

QString LogViewer::collectLogs()
{
	QStringList items;
	for (auto &message : lib::log::get_messages())
	{
		items.append(QString::fromStdString(message.format()));
	}
	return items.join('\n');
}

void LogViewer::saveToFile()
{
	auto fileName = QFileDialog::getSaveFileName(this,
		"Select location",
		QString("%1/spotify-qt-%2.log")
			.arg(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first())
			.arg(QDateTime::currentDateTime().toString("yyyyMMdd")),
		"Logs (*.log)");

	if (fileName.isEmpty())
		return;

	QFile out(fileName);
	out.open(QIODevice::WriteOnly);
	out.write(collectLogs().toUtf8());
	out.close();
}

void LogViewer::copyToClipboard()
{
	QApplication::clipboard()->setText(collectLogs());
}
