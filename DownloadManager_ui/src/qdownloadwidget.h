/***************************************************************************
 *   Copyright (C) 2008 by Bartek Tacza�a 								   *
 *   b@kontrasty.szczecin.pl   											   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef QDOWNLOADWIDGET_H
#define QDOWNLOADWIDGET_H
#include <QTreeWidget>
#include <QItemDelegate>
#include <vector>

class QPaintEvent;

class QDownloadWidget : public QTreeWidget
{
	Q_OBJECT
public:
	QDownloadWidget(QWidget * parent = 0);
	~QDownloadWidget(); 

    class QDownloadWidgetColumnInfo
    {
    public:
        QDownloadWidgetColumnInfo(int id, QString& colName, bool visible)
        {
            _id = id; _colName = colName; _visible = visible;
        }
        int getId() const { return _id; }
        const QString& getName() const { return _colName; }
        bool isVisible() const { return _visible; }

        void setVisible(bool visible) { _visible = visible; }

        static const QString settingsName;

        enum
        {
            ColumnId = 0,
            ColumnPath,
            ColumnFileSize,
            ColumnProgress,
            ColumnDownload
        };

    private:
        int _id;
        QString _colName;
        bool _visible;
    };

    typedef std::vector<QDownloadWidgetColumnInfo> ColumnCollection;

public slots:
    void StartPauseSelectedDownload();
    void StopSelectedDownload();    
    void RemoveSelectedDownload();
    void onConfigureColumns();
    
private:
    QDownloadWidget(const QDownloadWidget & ) ; // hidden 
    void InitializeColumns();
    void ReloadColumns(bool readSettings = false);
    void SaveColumns();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent * event );


    ColumnCollection m_columns;
    
};

namespace DownloadWidgetDelegates
{
    enum DownloadViewItemMargins
    {
        horizonatalMargin = 3,
        verticalMargin = 1,
    };
    
    class QDownloadIconedItemDelegate : public QItemDelegate
    {
	    Q_OBJECT
    public:
        QDownloadIconedItemDelegate(QObject *parent ) ; 
        virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const ;

    };

    class QDownloadProgressDelegate :	public QItemDelegate
    {
	    Q_OBJECT
    public:
        QDownloadProgressDelegate(QObject *parent ) : QItemDelegate(parent){}; 
	    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const ;
    };
};
#endif // QDOWNLOADWIDGET_H
