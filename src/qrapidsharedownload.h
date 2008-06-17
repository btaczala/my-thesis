#ifndef QRAPIDSHAREDOWNLOAD_H
#define QRAPIDSHAREDOWNLOAD_H
//

//
#include <QString>
#include <QHttp>
#include <QHttpRequestHeader>
#include <QDebug>
#include <QObject>
#include "debugutils.h"


/*

How send requests to rapidshare server
Send GET 
GET /files/122321322/Me__su____eyrum_vi__spilum_endalaust.part2.rar HTTP/1.1
with:
Host: rapidshare.com\r\n
Connection: Keep-Alive\r\n
Cookie: user=4625386-%57%4D%32%46%54%5A%67%78%35%59\r\n
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; ru) Opera 8.50\r\n
Referer: http://rapidshare.com/files/122321322/Me__su____eyrum_vi__spilum_endalaust.part2.rar\r\n 


then 
GET /files/122321322/Me__su____eyrum_vi__spilum_endalaust.part2.rar HTTP/1.1
with:
Host: rapidshare.com\r\n
Connection: Keep-Alive\r\n
User-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; ru) Opera 8.50\r\n
Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, application/x-shockwave-flash, *\/*
Referer: http://rapidshare.com/files/122321322/Me__su____eyrum_vi__spilum_endalaust.part2.rar\r\n

somewhere in aswer whe have 
30  19 20 42 d4 00 00 61 6e  6e 65 72 3c 2f 61 3e 3c   . B...an ner</a><
0040  2f 6c 69 3e 20 20 20 20  20 20 20 20 20 20 20 20   /li>             
0050  20 20 20 0d 0a 20 20 20  20 20 20 20 20 20 20 20      ..            
0060  20 20 20 20 20 3c 6c 69  3e 3c 61 20 68 72 65 66        <li ><a href
0070  3d 22 68 74 74 70 3a 2f  2f 72 61 70 69 64 73 68   ="http:/ /rapidsh
0080  61 72 65 2e 63 6f 6d 2f  61 67 62 2e 68 74 6d 6c   are.com/ agb.html
0090  22 3e 43 6f 6e 64 69 74  69 6f 6e 73 20 6f 66 20   ">Condit ions of 
00a0  75 73 65 3c 2f 61 3e 3c  2f 6c 69 3e 0d 0a 20 20   use</a>< /li>..  
00b0  20 20 20 20 20 20 20 20  20 20 20 20 20 20 3c 6c                  <l
00c0  69 20 73 74 79 6c 65 3d  22 62 6f 72 64 65 72 3a   i style= "border:
00d0  30 70 78 3b 22 3e 3c 61  20 68 72 65 66 3d 22 68   0px;"><a  href="h
00e0  74 74 70 3a 2f 2f 72 61  70 69 64 73 68 61 72 65   ttp://ra pidshare
00f0  2e 63 6f 6d 2f 69 6d 70  72 69 6e 74 2e 68 74 6d   .com/imp rint.htm
0100  6c 22 3e 49 6d 70 72 69  6e 74 3c 2f 61 3e 3c 2f   l">Impri nt</a></
0110  6c 69 3e 0d 0a 20 20 20  20 20 20 20 20 20 20 20   li>..            
0120  20 3c 2f 75 6c 3e 0d 0a  20 20 20 20 20 20 20 20    </ul>..         
0130  3c 2f 6c 69 3e 20 20 20  20 20 20 20 20 0d 0a 20   </li>         .. 
0140  20 20 20 3c 2f 75 6c 3e  0d 0a 20 20 20 20 3c 2f      </ul> ..    </
0150  74 64 3e 3c 2f 74 72 3e  3c 2f 74 61 62 6c 65 3e   td></tr> </table>
0160  0d 0a 3c 2f 64 69 76 3e  0d 0a 3c 61 20 68 72 65   ..</div> ..<a hre
0170  66 3d 22 68 74 74 70 3a  2f 2f 72 61 70 69 64 73   f="http: //rapids
0180  68 61 72 65 2e 63 6f 6d  22 3e 3c 69 6d 67 20 73   hare.com "><img s
0190  72 63 3d 22 2f 69 6d 67  32 2f 72 73 6c 6f 67 6f   rc="/img 2/rslogo
01a0  2e 67 69 66 22 20 77 69  64 74 68 3d 22 33 30 30   .gif" wi dth="300
01b0  22 20 68 65 69 67 68 74  3d 22 32 31 34 22 20 61   " height ="214" a
01c0  6c 74 3d 22 6c 6f 67 6f  22 20 2f 3e 3c 2f 61 3e   lt="logo " /></a>
01d0  0d 0a 3c 6e 6f 73 63 72  69 70 74 3e 3c 68 31 3e   ..<noscr ipt><h1>
01e0  54 68 69 73 20 70 61 67  65 20 6e 65 65 64 73 20   This pag e needs 
01f0  4a 61 76 61 53 63 72 69  70 74 2c 20 74 6f 20 64   JavaScri pt, to d
0200  69 73 70 6c 61 79 20 61  6c 6c 20 69 6e 66 6f 72   isplay a ll infor
0210  6d 61 74 69 6f 6e 20 63  6f 72 72 65 63 74 21 3c   mation c orrect!<
0220  2f 68 31 3e 3c 2f 6e 6f  73 63 72 69 70 74 3e 0d   /h1></no script>.
0230  0a 0d 0a 3c 64 69 76 20  69 64 3d 22 69 6e 68 61   ...<div  id="inha
0240  6c 74 62 6f 78 22 3e 0d  0a 3c 73 63 72 69 70 74   ltbox">. .<script
0250  20 74 79 70 65 3d 22 74  65 78 74 2f 6a 61 76 61    type="t ext/java
0260  73 63 72 69 70 74 22 3e  0d 0a 3c 21 2d 2d 0d 0a   script"> ..<!--..
0270  66 75 6e 63 74 69 6f 6e  20 61 6e 7a 65 69 67 65   function  anzeige
0280  6e 28 6e 61 6d 65 29 0d  0a 7b 0d 0a 20 20 20 20   n(name). .{..    
0290  76 61 72 20 65 6c 65 6d  20 3d 20 64 6f 63 75 6d   var elem  = docum
02a0  65 6e 74 2e 67 65 74 45  6c 65 6d 65 6e 74 42 79   ent.getE lementBy
02b0  49 64 28 22 70 22 2b 6e  61 6d 65 29 3b 0d 0a 20   Id("p"+n ame);.. 
02c0  20 20 20 69 66 20 28 65  6c 65 6d 2e 73 74 79 6c      if (e lem.styl
02d0  65 2e 64 69 73 70 6c 61  79 3d 3d 22 22 29 20 7b   e.displa y=="") {
02e0  0d 0a 20 20 20 20 20 20  20 20 65 6c 65 6d 2e 73   ..         elem.s
02f0  74 79 6c 65 2e 64 69 73  70 6c 61 79 3d 22 6e 6f   tyle.dis play="no
0300  6e 65 22 3b 0d 0a 20 20  20 20 7d 20 65 6c 73 65   ne";..     } else
0310  20 7b 0d 0a 20 20 20 20  20 20 20 20 65 6c 65 6d    {..         elem
0320  2e 73 74 79 6c 65 2e 64  69 73 70 6c 61 79 3d 22   .style.d isplay="
0330  22 3b 0d 0a 20 20 20 20  7d 0d 0a 20 20 20 20 76   ";..     }..    v
0340  61 72 20 65 6c 65 6d 32  20 3d 20 64 6f 63 75 6d   ar elem2  = docum
0350  65 6e 74 2e 67 65 74 45  6c 65 6d 65 6e 74 42 79   ent.getE lementBy
0360  49 64 28 22 70 62 22 2b  6e 61 6d 65 29 3b 0d 0a   Id("pb"+ name);..
0370  20 20 20 20 69 66 20 28  65 6c 65 6d 2e 73 74 79       if ( elem.sty
0380  6c 65 2e 64 69 73 70 6c  61 79 3d 3d 22 22 29 20   le.displ ay=="") 
0390  7b 0d 0a 20 20 20 20 20  20 20 20 65 6c 65 6d 32   {..         elem2
03a0  2e 73 72 63 20 3d 20 22  2f 69 6d 67 32 2f 70 66   .src = " /img2/pf
03b0  65 69 6c 5f 61 75 66 2e  6a 70 67 22 3b 0d 0a 20   eil_auf. jpg";.. 
03c0  20 20 20 7d 20 65 6c 73  65 20 7b 0d 0a 20 20 20      } els e {..   
03d0  20 20 20 20 20 65 6c 65  6d 32 2e 73 72 63 20 3d        ele m2.src =
03e0  20 22 2f 69 6d 67 32 2f  70 66 65 69 6c 5f 7a 75    "/img2/ pfeil_zu
03f0  2e 6a 70 67 22 3b 0d 0a  20 20 20 20 7d 0d 0a 7d   .jpg";..     }..}
0400  0d 0a 0d 0a 2f 2f 2d 2d  3e 0d 0a 3c 2f 73 63 72   ....//-- >..</scr
0410  69 70 74 3e 0d 0a 3c 68  31 3e 46 49 4c 45 20 44   ipt>..<h 1>FILE D
0420  4f 57 4e 4c 4f 41 44 3c  2f 68 31 3e 0d 0a 3c 64   OWNLOAD< /h1>..<d
0430  69 76 20 63 6c 61 73 73  3d 22 6b 6c 61 70 70 62   iv class ="klappb
0440  6f 78 22 3e 0d 0a 20 20  20 20 59 6f 75 20 77 6f   ox">..     You wo
0450  75 6c 64 20 6c 69 6b 65  20 74 6f 20 64 6f 77 6e   uld like  to down
0460  6c 6f 61 64 20 74 68 65  20 66 6f 6c 6c 6f 77 69   load the  followi
0470  6e 67 20 66 69 6c 65 3a  3a 0d 0a 20 20 20 20 3c   ng file: :..    <
0480  70 20 63 6c 61 73 73 3d  22 64 6f 77 6e 6c 6f 61   p class= "downloa
0490  64 6c 69 6e 6b 22 3e 68  74 74 70 3a 2f 2f 72 61   dlink">h ttp://ra
04a0  70 69 64 73 68 61 72 65  2e 63 6f 6d 2f 66 69 6c   pidshare .com/fil
04b0  65 73 2f 31 32 32 33 32  31 33 32 32 2f 4d 65 5f   es/12232 1322/Me_
04c0  5f 73 75 5f 5f 5f 5f 65  79 72 75 6d 5f 76 69 5f   _su____e yrum_vi_
04d0  5f 73 70 69 6c 75 6d 5f  65 6e 64 61 6c 61 75 73   _spilum_ endalaus
04e0  74 2e 70 61 72 74 32 2e  72 61 72 20 3c 66 6f 6e   t.part2. rar <fon
04f0  74 20 73 74 79 6c 65 3d  22 63 6f 6c 6f 72 3a 23   t style= "color:#
0500  38 45 39 30 38 46 3b 22  3e 7c 20 33 34 37 31 34   8E908F;" >| 34714
0510  20 4b 42 3c 2f 66 6f 6e  74 3e 3c 2f 70 3e 0d 0a    KB</fon t></p>..
0520  20 20 20 20 3c 74 61 62  6c 65 20 63 65 6c 6c 70       <tab le cellp
0530  61 64 64 69 6e 67 3d 35  3e 0d 0a 20 20 20 20 20   adding=5 >..     
0540  20 20 20 3c 74 72 3e 0d  0a 20 20 20 20 20 20 20      <tr>. .       
0550  20 20 20 20 20 3c 74 64  3e 41 72 65 20 79 6f 75        <td >Are you
0560  20 61 20 50 72 65 6d 69  75 6d 20 55 73 65 72 3f    a Premi um User?
0570  20 4c 65 74 92 73 20 73  74 61 72 74 20 64 6f 77    Let.s s tart dow
0580  6e 6c 6f 61 64 69 6e 67  2e 3c 2f 74 64 3e 0d 0a   nloading .</td>..
0590  20 20 20 20 20 20 20 20  20 20 20 20 3c 66 6f 72                <for
05a0  6d 20 61 63 74 69 6f 6e  3d 22 68 74 74 70 3a 2f   m action ="http:/						<< ANSWER !!
05b0  2f 72 73 33 35 30 2e 72  61 70 69 64 73 68 61 72   /rs350.r apidshar						<< ANSWER !!
05c0  65 2e 63 6f 6d 2f 66 69  6c 65 73 2f 31 32 32 33   e.com/fi les/1223						<< ANSWER !!
05d0  32 31 33 32 32 2f 4d 65  5f 5f 73 75 5f 5f 5f 5f   21322/Me __su____
05e0  65 79 72 75 6d 5f 76 69  5f 5f                     eyrum_vi __ 

SECOND PACKAGE
00 50 39 7d fb 02  6e 65 cc 1c 2d a3 50 10   ...P9}.. ne..-.P.
0030  19 20 59 3c 00 00 73 70  69 6c 75 6d 5f 65 6e 64   . Y<..sp ilum_end
0040  61 6c 61 75 73 74 2e 70  61 72 74 32 2e 72 61 72   alaust.p art2.rar						<< ANSWER !!
0050  22 20 6d 65 74 68 6f 64  3d 22 70 6f 73 74 22 3e   " method ="post">						<< ANSWER !!
0060  0d 0a 20 20 20 20 20 20  20 20 20 20 20 20 3c 69   ..             <i						<< ANSWER !!
0070  6e 70 75 74 20 74 79 70  65 3d 68 69 64 64 65 6e   nput typ e=hidden						<< ANSWER !!
0080  20 6e 61 6d 65 3d 22 64  6c 2e 73 74 61 72 74 22    name="d l.start"						<< ANSWER !!
0090  20 76 61 6c 75 65 3d 22  50 52 45 4d 49 55 4d 22    value=" PREMIUM"						<< ANSWER !!
00a0  20 2f 3e 0d 0a 20 20 20  20 20 20 20 20 20 20 20    />..            						<< ANSWER !!
00b0  20 3c 74 64 3e 3c 69 6e  70 75 74 20 73 74 79 6c    <td><in put styl
00c0  65 3d 22 77 69 64 74 68  3a 31 30 30 70 78 3b 22   e="width :100px;"
00d0  20 74 79 70 65 3d 22 73  75 62 6d 69 74 22 20 76    type="s ubmit" v
00e0  61 6c 75 65 3d 22 50 72  65 6d 69 75 6d 22 20 2f   alue="Pr emium" /
00f0  3e 3c 2f 74 64 3e 20 20  20 20 20 20 20 20 20 20   ></td>           

the we post

POST /files/122321322/Me__su____eyrum_vi__spilum_endalaust.part2.rar HTTP/1.1\r\n
Host: rs350.rapidshare.com\r\n
Connection: Keep-Alive\r\n
Cookie: user=4625386-%57%4D%32%46%54%5A%67%78%35%59\r\n
User-Agent: Mozilla/4.0 (compatible; Synapse)\r\n
Content-Type: application/x-www-form-urlencoded\r\n
Content-Length: 16
Referrer: http://rapidshare.com/files/122321322/Me__su____eyrum_vi__spilum_endalaust.part2.rar\r\n


*/
class QRapidshareDownload : public QObject
{
Q_OBJECT
public:
	QRapidshareDownload()
	{
		;
	}
	QRapidshareDownload(const QString & _UrlFileAddress = QString("") );
	virtual 					~QRapidshareDownload();
	
	void 						Download(const QString & _addr = QString("") );
	void						SetUrlFileAddress(const QString & _addr ) ;
	/// ???? implement or not ? 
	static	void				DownloadFile(const QString & _addr);
	
private:
	QString									m_UrlFileAddress; 
	std::auto_ptr<QHttp>					m_apHttpObj;
	std::auto_ptr<QHttpRequestHeader>		m_apHttpRequestHeader;
	bool									m_bIsPrepared;
	QString									m_HostName;
	QString									m_PathOnServer;
	QString									m_RequestType;	
	void 									ParseAddress();	
	void 									Post();
	QString									Cookie();
private slots:
	void 									requestStarted(const int & idReq) ;
	void 									requestFinished(const int & idReq, const bool & isFalse) ;
	void 									stateChanged(const int & iState) ;
	void 									dataSendProgress(const int & done, const int & total) ;
	void 									dataReadProgress(const int & done, const int & total) ;
	void									responseHeaderReceived( const QHttpResponseHeader & resp) ;  
};
#endif
