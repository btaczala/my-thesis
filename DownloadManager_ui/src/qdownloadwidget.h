/***************************************************************************
 *   Copyright (C) 2008 by Bartek Taczała                                  *
 *   b@kontrasty.szczecin.pl                                               *
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
#include <QHeaderView>
#include <vector>
#include <QPointer>
#include <idownload.h>
#include <QDebug>
class QPaintEvent;
class QContextMenuEvent;
class DownloadManager;
#define UNSET_COL (-1)

class QDownloadHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    QDownloadHeaderView(QWidget* parent = 0): QHeaderView(Qt::Horizontal, parent)
    {
        setResizeMode(QHeaderView::Interactive); 
        setMovable(true);
    }
protected:
    virtual void contextMenuEvent ( QContextMenuEvent * event ) { emit contextMenu(event); }
signals:
    void contextMenu(QContextMenuEvent* event);
};

class QTestWidget : public QWidget
{
    Q_OBJECT
public:
    QTestWidget(QWidget* parent = 0);
};

namespace DownloadWidgetDelegates
{
    enum DownloadViewItemMargins
    {
        horizonatalMargin = 3,
        verticalMargin = 1,
    };

    class DownloadItemDelegate : public QItemDelegate
    {
        Q_OBJECT
    public:
        DownloadItemDelegate(QObject* parent);
        virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const ;
        virtual QSize sizeHint( const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    public slots:
        void itemActivated(const QModelIndex& index);

    protected:
        void drawStandardItem(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const ;
        void drawDetailedItem(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const ;
    private:
        QMap<int, QWidget*> m_details;
    };

};

class QDownloadWidget : public QTreeWidget
{
    Q_OBJECT
public:
    QDownloadWidget(QWidget * parent = 0);
    virtual ~QDownloadWidget(); 

    /**
     * If engine found url will be added to download manager list, and to treewidget. 
     * @param url 
     * @param fileDestination 
     */
    void addDownload(const QString & url, const QString & fileDestination); 
    
    static const QString ListenerName ; 
    class QDownloadWidgetColumnInfo
    {
    public:
        QDownloadWidgetColumnInfo( int id, const QString& colName, bool visible = true, int width = 100, int visualIndex = UNSET_COL) // has to be const when passing temporary object
        {
            _id = id; _colName = colName; _visible = visible; _width = width; _visualIndex = visualIndex;
        }

        enum ColumnType // gcc does not support unnamed enums ( ? ) 
        {
            ColumnId = 0,
            ColumnPath,
            ColumnFileSize,
            ColumnProgress,
            ColumnDownload,
            ColumnElapsedTime,
            ColumnEstimatedTime,
            ColumnDownloadRate,
        };

        int getId() const { return _id; }
        const QString& getName() const { return _colName; }
        bool isVisible() const { return _visible; }
        int getWidth() const { return _width; }
        int getVisualIndex() const { return _visualIndex; }

        void setVisible(bool visible) { _visible = visible; }
        void setWidth(int width) { _width = width; }
        void setVisualIndex(int visualIndex) {_visualIndex = visualIndex; }

        static const int VERSION;
    private:
        int _id;
        int _visualIndex;
        QString _colName;
        bool _visible;
        int _width;
    };

    typedef std::vector<QDownloadWidgetColumnInfo> ColumnCollection;

    enum ItemType
    {
        StandardItem,
        DetailedItem
    };

public slots:
    void StartPauseSelectedDownload();
    void StopSelectedDownload();
    void RemoveSelectedDownload();
    void onConfigureColumns();
    void columnChanged(QDownloadWidget::QDownloadWidgetColumnInfo* column);
    void contextMenu(QContextMenuEvent * event );
    void columnHide(); 
    void sectionMoved(int logicalIndex, int oldVisualIndex, int newVisualIndex);

    void globalProgressChanged( int value ) ; 
    
    void downloadDoneAt( int position );
    void downloadOnHold( int position ) ; 
    //void bytesReadAt( int position , int read , int total );
   //void downloadRateAt( int position , const QString & downloadRate );
    //void elapsedTimeAt( int position , unsigned int timeElapsed );
    
    void statusChanged( int position, DownloadState::States status );
    void progressInfoAt( int, const ProgressInfo& _info );
    void downloadAdded( int newPosition ) ;
    void downloadRemoved( int position ) ;
private:
    QDownloadWidget(const QDownloadWidget & ) ; // hidden 
    
    void connectWithDownloadManager() ;
    void disconnectFromDownloadManager() ;
    void initializeColumns();
    void reloadColumns(bool readSettings = false);
    void saveColumns();
    void restoreColumns();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void contextMenuEvent(QContextMenuEvent * event );
    virtual void keyPressEvent(QKeyEvent* event);
protected slots:
    void columnResized(int column, int oldSize, int newSize);
protected:
    ColumnCollection m_columns;
    QPointer<QMenu>  m_pContextMenu ; 
    int              m_CurrentColumnID ; 
    DownloadWidgetDelegates::DownloadItemDelegate* m_downloadItemDelegate;
    DownloadManager*  m_pDownloadManager ; 

};
#endif // QDOWNLOADWIDGET_H
