
/************************************************************************/
/*																		*/
/* FILE		rf_ismreco.cc												*/
/* AUTHOR	Yuren Zhang													*/
/* EMAIL	kiekergaard@gmail.com;yuren.zhang@ia.ac.cn					*/
/*																		*/
/* CONTENT	Implement of the UI of the random forest and ISM based		*/
/*		object detecton program.										*/
/* BEGIN	2013.2.1													*/
/* LAST CHANGE	2013.2.4												*/
/************************************************************************/

/****************/
/*	Include		*/
/****************/
#include <time.h>
#include <fstream>
#include <sstream>
#include <QWidget>
#include <QtGui>
#include <qtimgbrowser.hh>
#include "featExtract.hh"
#include "kmeans.hh"
/****************/
/*	Defintions	*/
/***************/
FeatExtract::FeatExtract(QWidget *parent, const char *name):QWidget( parent)
{	
  vsHough = NULL;
  m_pFeatures = NULL;
  m_pClusterer = NULL;
	/*----------------------*/
	/*	Making the Title	*/
	/*----------------------*/
  QFont ft;
  ft.setPointSize(20);
  QLabel *logo = new QLabel(	(STR_NAME + " " + STR_VERSION).c_str(),
				this);
  logo->setFont(ft);
  /*--------------------------------------------*/
  /*	Make a patch sampling setting group		*/
  /*--------------------------------------------*/
	QVBoxLayout *vbfcCue = new QVBoxLayout;
	QGroupBox *gbfcCue = new QGroupBox();
	QLabel    *lbfcCue = new QLabel("<b>Set the Patch Sampling Params</b>");
	vbfcCue->addWidget(lbfcCue);
	vbfcCue->addWidget(gbfcCue);
	FeatureGUI *guiCue = m_fcCue.createGUI( gbfcCue, "cue1");
	
  /*--------------------------------------------*/
  /*	Make a codebook display(Ref. Leibe)		*/
  /*--------------------------------------------*/
  // 
  QGroupBox *gbIconBox = new QGroupBox;
  gbIconBox->setTitle("Leaf Nodes");
  qClassView = new QtClusterView( gbIconBox, "codebook view" );
	gbIconBox->setFixedSize(700,200);
  /*--------------------------------------------*/
  /*	Make a image display window(Ref. Leibe)	*/
  /*--------------------------------------------*/
	QGroupBox *boxImg = new QGroupBox;
	QHBoxLayout *hbImgBox = new QHBoxLayout;
	QFrame	*fSource = new QFrame();
	QFrame	*fResult = new QFrame();
	rsSourceImg = new QtResizeImg(fSource, "Source Image");
	rsResultImg = new QtResizeImg(fResult, "Result Image");
	hbImgBox->addWidget(rsSourceImg);
	hbImgBox->addWidget(rsResultImg); 
	boxImg->setLayout(hbImgBox);
	boxImg->setFixedSize(700,200);
  /*----------------------------------------*/
  /*	Put these two windows into on QVBox	*/
  /*----------------------------------------*/
	QVBoxLayout *vbShow = new QVBoxLayout;
	vbShow->addWidget( boxImg);
	vbShow->addWidget( gbIconBox );
  /*----------------------------------------*/
  /*	Make the Load Image button      	*/
  /*----------------------------------------*/
	QVBoxLayout *vhButtons = new QVBoxLayout;
	QPushButton *btLoadImage = new QPushButton("Load Images and Extract Features");
	vhButtons->addWidget(btLoadImage);
	QPushButton *btLoadFeatures = new QPushButton("Load Features");
	vhButtons->addWidget(btLoadFeatures);
	QPushButton *btCluster = new QPushButton("Cluster");
	vhButtons->addWidget(btCluster);
	QPushButton *btLoadCenters = new QPushButton("Load Centers");
	vhButtons->addWidget(btLoadCenters);
	QPushButton *btGetAssignMent = new QPushButton("Get Assignment");
	vhButtons->addWidget(btGetAssignMent);

	connect( btLoadImage, SIGNAL( clicked() ),
			 this, SLOT( loadImageSet() ));
	connect( btLoadFeatures, SIGNAL( clicked() ),
			 this, SLOT( loadFeatureSet() ));
	connect( btCluster, SIGNAL( clicked() ),
			 this, SLOT( cluster()));
	connect( btLoadCenters, SIGNAL( clicked() ),
			 this, SLOT( loadCenters()) );
	connect( btGetAssignMent, SIGNAL( clicked()),
			 this, SLOT( getAssignmentandSave()));
  /*----------------------------*/
  /*	Set the Global grid		*/
  /*----------------------------*/
	QGridLayout	*grid = new QGridLayout();
	grid->addWidget( logo, 0, 0, Qt::AlignHCenter);
	grid->addLayout( vbShow, 2, 1 );
	grid->addLayout( vhButtons, 1, 1 );
	gbfcCue->setFixedSize( 300,600 );
	grid->addLayout( vbfcCue, 2, 0);
	this->setLayout(grid);
	QGroupBox *blankBox = new QGroupBox;
	grid->addWidget(blankBox, 3,0);
	grid->setRowStretch(3, 10);
	rsSourceImg->loadImage( IMG_DIR+"/aeroplane/2008_000033.png");
	rsSourceImg->display();
	rsResultImg->loadImage( IMG_DIR+"/aeroplane/2008_000037.png");
	rsResultImg->display();	
    QImage img(IMG_DIR+"/aeroplane/2008_000033.png");
	qClassView->addImageThumbnail(img,"The first","The first");
}

void FeatExtract::loadImageSet()
{
  m_vpsPatch4Train.clear();
  cleanValidateVector();

  QStringList qslFileList;
  QStringList qslFileListSeveral;
  QString     lastDir( IMG_DIR );
  qslFileListSeveral = QFileDialog::getOpenFileNames(
                           "Images (*.png *.ppm *.pgm *.jpg);;All files (*.*)",
                           lastDir, this, "imageset", "Select Images" );
  
  if (qslFileListSeveral.isEmpty())
    return;
  
  lastDir = qslFileListSeveral[0];
  cout << "  ...'" << qslFileListSeveral[0].toStdString() << "'... [" 
       << qslFileListSeveral.size() << "]" << endl;
  
  /* add entries for the image class */
  int nCount = (int)qslFileListSeveral.size();
  bool ok = true;
  while( ok ) {
    // ask for more images
    switch( QMessageBox::information( this, STR_NAME.c_str(),
                                      "Load more images?",
                                      "&Yes", "&No", "&Cancel",
                                      0, 2 ) ) {
    case 0: // Yes clicked
      qslFileList = QFileDialog::getOpenFileNames(
                       "Images (*.png *.ppm *.pgm *.jpg);;All files (*.*)",
                       lastDir, this, "filedia", "Select More Files" );
      if ( !qslFileList.isEmpty() ) {
        lastDir = qslFileList[0];
        qslFileListSeveral += qslFileList; //Append
      } else
        ok = false;
      break;
      
    case 1: // No clicked
      ok = false;
      break;
      
    case 2: // Cancel clicked
      ok = false;
      return;
      break;
    }
  }
  m_qslFileList = qslFileListSeveral;

  for(unsigned i=0; i < qslFileListSeveral.count(); i++) {
    cout<<" Processing the "<< i <<"of "<<qslFileListSeveral.count()<<"images "<<endl;
    processImage( qslFileListSeveral[i], i);
  } 
  vector<OpGrayImage> vImagePatches = getImagePatches();
  displayPatchesForBrowsing( vImagePatches ); 
}

void FeatExtract::loadFeatureSet()
{
	clearFeature();
	QStringList featurefilenames = loadFileSet(" Features (*.feat);;All files (*.*)", IMG_DIR);
	m_featureFileList = featurefilenames;
	unsigned int featurenum = getFeatureNum(featurefilenames);
	if(m_pFeatures != NULL)
	{
		delete[] m_pFeatures;
		m_pFeatures = NULL;
	}
	m_pFeatures = new float[featurenum * SIFT_FEATURE_DIMENSION];
	m_FeatureValueIndex = 0;
	for(unsigned int i=0; i < featurefilenames.count(); i++)
	{
		cout<<" Loading the "<< i+1 <<"of "<<featurefilenames.count()<<"feature files "<<endl;
		loadFeature(featurefilenames[i]);
	}
	if(m_FeatureValueIndex != featurenum*SIFT_FEATURE_DIMENSION)
	{
		cout<<"Errer in loadFeatureSet: not enough data have been loaded: "<<endl;
		cout<<"Data(float) need to be loaded: "<<featurenum*SIFT_FEATURE_DIMENSION<<endl;
		cout<<"Data(float) actually loaded: "<<m_FeatureValueIndex<<endl;
	}
}

void FeatExtract::cluster()
{
	clock_t start,finish;
	if(m_FeatureValueIndex == 0)
	{
		cout<<" Error in cluster(): no features have been loaded"<<endl;
		return;
	}
	cout<<"  cluster(), clustering..."<<endl;
	if(m_pClusterer != NULL)
	{
		delete m_pClusterer;
		m_pClusterer = NULL;
	}
	start = clock();
	m_pClusterer = new KMeansClustering<float>(KMEANS_Lloyd, KMEANS_L2, DEFAULT_CENTER_SIZE, 100, SIFT_FEATURE_DIMENSION, m_FeatureValueIndex/SIFT_FEATURE_DIMENSION, (void*)m_pFeatures);
	finish = clock();
	cout<<"  cluster(), clustering lasted for "<<(double)(finish-start)/CLOCKS_PER_SEC<<" seconds"<<endl;
	cout<<"  cluster(), saving centers and assignments..."<<endl;
	m_featureCenters = m_pClusterer->getCenters();
	saveFeatures(m_featureCenters, "centers.feat");
	
	getAssignmentandSave();
}

void FeatExtract::getAssignmentandSave()
{
	for(uint32_t i=0; i<m_featureFileList.count(); i++)
	{
		assignFeaturestoCodeWordandSave(m_featureFileList[i]);
		cout<<"   assigning the "<<i <<" of "<<m_featureFileList.count()<<" feature file"<<endl;
	}
	cout<<" assignment done.";
}

void FeatExtract::assignFeaturestoCodeWordandSave(QString featurefilename)
{
	uint32_t featurenum = getFeatureNum(featurefilename);	
	uint32_t buffersize = featurenum * SIFT_FEATURE_DIMENSION;
	float* rawdata = new float[buffersize];
	loadFeaturetoBuffer( featurefilename, rawdata, buffersize);

	vector< DataPointAssignment<float> >  assignment = m_pClusterer->getAssignment((void*)rawdata, featurenum);

	string assginfilename = replacesuffix(featurefilename.toStdString(), ".assign");
	ofstream ofs(assginfilename.c_str());
	for(int i=0; i<assignment.size();i++)
	{
		ofs << assignment[i].assignment << endl;
	}
	delete[] rawdata;
	rawdata = NULL;
	ofs.close();
}

uint32_t FeatExtract::getFeatureNum(const QStringList& featurefilenames)
{
	const int LINE_LENTH = 10000;
	char str[LINE_LENTH];
	unsigned int featurenum = 0;
	for(unsigned int i=0; i<featurefilenames.count(); i++)
	{
		ifstream ifs(featurefilenames[i].toStdString().c_str());
		while(ifs.getline(str, LINE_LENTH))
			featurenum++;
		ifs.close();
	}
	return featurenum;
}

void FeatExtract::loadCenters()
{
	QStringList centerfile = loadFileSet(" Features (*.feat);;All files (*.*)", "./");
	float* rawdata = new float[DEFAULT_CENTER_SIZE * SIFT_FEATURE_DIMENSION];
	loadFeaturetoBuffer( centerfile[0], rawdata, DEFAULT_CENTER_SIZE * SIFT_FEATURE_DIMENSION); 
	if(clustererExists())
	{
		m_pClusterer->setCenters(rawdata, DEFAULT_CENTER_SIZE);
	}
	else
	{
		m_pClusterer = new KMeansClustering<float>(SIFT_FEATURE_DIMENSION);
		m_pClusterer->setCenters(rawdata, DEFAULT_CENTER_SIZE);
	}
}

	
bool FeatExtract::clustererExists()
{
	if(m_pClusterer != NULL )
		return true;
	else return false;
}
void FeatExtract::loadFeature(QString filename)
{
	ifstream ifs(filename.toStdString().c_str());
	float data;
	unsigned int datacount = 0;
	while(ifs >> data)
	{
		m_pFeatures[m_FeatureValueIndex] = data;
		m_FeatureValueIndex++;
	}
	ifs.close();
}
void FeatExtract::clearFeature()
{
	if(m_pFeatures != NULL)
	{
		delete[] m_pFeatures;
		m_pFeatures = NULL;
	}
	m_FeatureValueIndex = 0;
}
void FeatExtract::loadFeaturetoBuffer(const QString& filename, float* ptr, uint32_t datanum)
{
	uint32_t index = 0;
	ifstream ifs(filename.toStdString().c_str());
	double data;
	while(ifs >> data)
	{
		ptr[index] = data;
		index++;
	}	
	if(index != datanum)
		cout<<"Error in loadFeaturetoBuffer: the number of values loaded is wrong"<<endl;
	ifs.close();
}

uint32_t FeatExtract::getFeatureNum(const QString& filename)
{
	uint32_t featurenum = 0;
	const uint32_t LINE_LENTH = 10000;
	char str[LINE_LENTH];
	ifstream ifs(filename.toStdString().c_str());
	while(ifs.getline(str, LINE_LENTH))
		featurenum++;
	ifs.close();
	return featurenum;
}

void FeatExtract::processImage(QString qstr,  uint32_t nImageIndex)
/*=================================================================*/
/* Process a new image, i.e. load it and extract patches using the */
/* methods selected in the used interface. The extracted patches   */
/* are then stored in m_vpsPatch4Train.                            */
/*=================================================================*/
{
  cleanValidateVector();
  loadImage( qstr );
  qApp->processEvents(); // finish drawing
  QString annofile(qstr);
  annofile.replace(annofile.indexOf("."), 10, QString(".anno"));
  annofile.replace(QString("JPEGImages"), QString("Annotations"));
  loadAnnotation(annofile.toStdString());
  extractFeature();
  collectPatchSpatialInfo();
  saveFeatures();
  saveSpatialInfo();
  saveKeyPoints();
//  drawInterestPointsEllipse();
  qApp->processEvents();
}

void FeatExtract::loadAnnotation(string filename)
{
	string content;
	istringstream iss;
	fstream fAnno;
	fAnno.open(filename.c_str(), fstream::in);
	while(getline(fAnno, content))
	{
		Annotation anno;
		iss.str(content);
		iss >> anno.classno >> anno.isOccluded >> anno.x1 >> anno.y1 >> anno.x2 >> anno.y2;
		m_vAnnotation.push_back(anno);
	}
	fAnno.close();
}
 

void FeatExtract::extractFeature()
{
  QImage qimg = m_grayImg.getQtImage();
  cout<<"   Start Processing image..."<<endl;
  m_fcCue.processImage( m_sImgFullName, m_grayImg, m_grayImgMap, qimg, 
                        m_nFeatureType, m_vPoints, m_vPointsInside, 
                        m_vPatch, m_vFeature);
  for(int i=0; i<m_vFeature.size(); i++)
  {
	  vector<float> fv;
	  for(int j=0; j<m_vFeature[i].numDims();j++)
		  fv.push_back(m_vFeature[i].at(j));
	  m_vFeature_raw.push_back(fv);
  }
}

void FeatExtract::collectPatchSpatialInfo()
{
	for(int i=0; i<m_vFeature.size(); i++)
	{
		if(!collectPatchSpatialInfo_UnOccluded(i))
			collectPatchSpatialInfo_Occluded(i);
	}
}


// 
bool FeatExtract::collectPatchSpatialInfo_UnOccluded(int patchindex)
{
	bool isInUnoccluded = false;
	for(int corespondingAnno=0; corespondingAnno<m_vAnnotation.size(); corespondingAnno++)
	{
		if(m_vAnnotation[corespondingAnno].isOccluded==0 && isInsideBBox(patchindex, corespondingAnno))
		{
			isInUnoccluded = true;
			pushbackSpatialInfo(patchindex, corespondingAnno);
		}
	}
	return isInUnoccluded;
}

// 
void FeatExtract::collectPatchSpatialInfo_Occluded(int patchindex)
{
	for(int corespondingAnno=0; corespondingAnno<m_vAnnotation.size();corespondingAnno++)
	{
		if(m_vAnnotation[corespondingAnno].isOccluded==1 && isInsideBBox(patchindex, corespondingAnno))
		{
			pushbackSpatialInfo(patchindex, corespondingAnno);
		}
	}
}

bool FeatExtract::isInsideBBox(int patchindex, int annoindex)
{
	//TODO: implement the getPatchSize Function 
	// patch size is half the width of the patch
	int patchsize = static_cast<int>(m_fcCue.getPatchScaleRatio()*m_vPoints[patchindex].scale);
	return((m_vPoints[patchindex].x-patchsize) >= m_vAnnotation[annoindex].x1 &&
		   (m_vPoints[patchindex].y-patchsize) >= m_vAnnotation[annoindex].y1 &&
		   (m_vPoints[patchindex].x+patchsize) <= m_vAnnotation[annoindex].x2 &&
		   (m_vPoints[patchindex].y+patchsize) <= m_vAnnotation[annoindex].y2);
}

void FeatExtract::pushbackSpatialInfo(int patchindex, int annoindex)
{
	SpatialInfo spinfo;
	Annotation anno = m_vAnnotation[annoindex];
	spinfo.patchno = patchindex;
	spinfo.classno = anno.classno;
	spinfo.width_scale_ratio = static_cast<float>(anno.x2 - anno.x1)/m_vPoints[patchindex].scale;
	spinfo.height_scale_ratio = static_cast<float>(anno.y2 - anno.y1)/m_vPoints[patchindex].scale;
	spinfo.offset_x_scale_ratio = static_cast<float>((anno.x1 + anno.x2)/2-m_vPoints[patchindex].x)/m_vPoints[patchindex].scale;
	spinfo.offset_y_scale_ratio = static_cast<float>((anno.y1 + anno.y2)/2-m_vPoints[patchindex].y)/m_vPoints[patchindex].scale;
	m_vSpatialInfo.push_back(spinfo);
}

void FeatExtract::saveFeatures()
{
	string filename = makeFeatureFileName();
	fstream featurefile;
	featurefile.open(filename.c_str(), fstream::out);
	for(int i=0; i<m_vFeature.size(); i++)
	{
		for(int j=0; j<m_vFeature[i].numDims(); j++)
			featurefile<<m_vFeature[i].at(j)<<" ";
		featurefile<<endl;
	}
	featurefile.close();
}

void FeatExtract::saveFeatures(const vector< vector<float> >& features, const string& filename)
{
	fstream featurefile;
	featurefile.open(filename.c_str(), fstream::out);
	for(int i=0; i<features.size(); i++)
	{
		for(int j=0; j<features[i].size(); j++)
			featurefile<<features[i][j]<<" ";
		featurefile<<endl;
	}
	featurefile.close();
}

string FeatExtract::makeFeatureFileName()
{
	size_t suffixpos = m_sImgFullName.rfind(".");
	string filename = m_sImgFullName.replace(suffixpos, string::npos, ".feat");
	return filename;
}

void FeatExtract::saveSpatialInfo()
{
	string filename = makeSpatialInfoFileName();
	fstream featurefile;
	featurefile.open(filename.c_str(), fstream::out);
	for(int i=0; i<m_vSpatialInfo.size(); i++)
	{
		featurefile	<<m_vSpatialInfo[i].patchno<<" "
			  		<<m_vSpatialInfo[i].classno<<" "
					<<m_vSpatialInfo[i].width_scale_ratio<<" "
					<<m_vSpatialInfo[i].height_scale_ratio<<" "
					<<m_vSpatialInfo[i].offset_x_scale_ratio<<" "
					<<m_vSpatialInfo[i].offset_y_scale_ratio<<endl;
	}
	featurefile.close();
}

string FeatExtract::makeSpatialInfoFileName()
{
	size_t suffixpos = m_sImgFullName.rfind(".");
	string filename = m_sImgFullName.replace(suffixpos, string::npos, ".spinfo");
	return filename;
}

void FeatExtract::saveKeyPoints()
{
	string filename = makeKeyPointFileName();
	fstream keypointfile;
	keypointfile.open(filename.c_str(), fstream::out);
	for(int i=0; i<m_vPoints.size(); i++)
	{
		keypointfile	<< i <<" "
			 		<< m_vPoints[i].x <<" "
					<< m_vPoints[i].y <<" "
					<< m_vPoints[i].scale*m_fcCue.getPatchScaleRatio()<<endl;
	}
	keypointfile.close();

}

string FeatExtract::makeKeyPointFileName()
{
	size_t suffixpos = m_sImgFullName.rfind(".");
	string filename = m_sImgFullName.replace(suffixpos, string::npos, ".key");
	return filename;
}

vector<QImage> FeatExtract::grayImageVector2QImageVector(const vector<OpGrayImage>& grayImageVector)
{
	vector<QImage> vQImg;
	for(vector<OpGrayImage>::const_iterator itr = grayImageVector.begin(); itr != grayImageVector.end(); itr++)
	{
		vQImg.push_back(itr->getQtImage());
	}
	return vQImg;
}

void FeatExtract::cleanValidateVector()
{
	m_vAnnotation.clear();
	m_vFeature.clear();
	m_vPatch.clear();
	m_vSpatialInfo.clear();
}
	
void FeatExtract::loadImage(QString qstr)
/*=================================================================*/
/* Load a new test image (including its segmentation mask, if one  */
/* exists). Specifically, the image and mask are stored in  class  */
/* member, and correspondig directories strings are stored.        */
/*=================================================================*/
{
  cout << "  Clusterer::loadImage() called." << endl;
  m_img.load( qstr );
  
  /* store image name (abbreviated) */
  unsigned pos;
  string tmpstr( qstr.latin1() );
  if ( ( pos = tmpstr.rfind( "/" )) != string::npos ) {
    if ( tmpstr.at(pos+3) != (int) string::npos )
      tmpstr = tmpstr.substr( pos+1, 3);
    else
      tmpstr = "Img";

  } else
    tmpstr = "Img";
  m_sImgNameShort = tmpstr;
  
  /* store long image name */
  string sFileName( qstr.latin1() );
  int    len  = (int)sFileName.length();
  int    pos1 = (int)sFileName.rfind("/");
  int    pos2 = (int)sFileName.rfind("/", pos1-1);
  m_sImgName = "";
  m_sImgPath = "";
  m_sImgFullName = sFileName;
  if( pos1 != (int)string::npos ) {
    m_sImgName = sFileName.substr( pos1+1, len-pos1 );
    if( pos2 != (int)string::npos )
      m_sImgPath = sFileName.substr( pos2+1, pos1-pos2-1 );
  }

  /* convert to OpGrayImage structure */
  OpGrayImage grayImg( m_img );
  m_grayImg = grayImg;
  
  /* display */
  rsSourceImg->loadImage( m_img );
  rsSourceImg->display();
  
  m_bPicLoaded = true;
  cout << "  done." << endl;
}

void FeatExtract::drawInterestPoints()
/*******************************************************************/
/* Draw the interest points returned by the Int.Pt. detector.      */
/*******************************************************************/
{
  if ( m_vPoints.empty() )
    return;
  
  // display original image first (clean any drawings)
  rsResultImg->display();
  for( int i=0; i <(int)m_vPoints.size(); i++) {
    rsResultImg->drawCross( m_vPoints[i].x, m_vPoints[i].y,
                            QColor::QColor(255,255,0) );
    if( m_vPoints[i].scale > 1.0 )
      rsResultImg->drawCircle( m_vPoints[i].x, m_vPoints[i].y,
                               m_vPoints[i].scale*3.0,
                               QColor::QColor(255,255,0) );
  }
  qApp->processEvents();
}

void FeatExtract::drawInterestPointsEllipse()
/*******************************************************************/
/* Draw the interest points returned by the Int.Pt. detector.      */
/*******************************************************************/
{
  if ( m_vPoints.empty() )
    return;
 
  cout << "  Drawing " << m_vPoints.size() << " points..." << endl;
 
  // display original image first (clean any drawings)
  rsResultImg->display();
  for( int i=0; i <(int)m_vPoints.size(); i++) {
    rsResultImg->drawCross( m_vPoints[i].x, m_vPoints[i].y,
                            QColor::QColor(255,255,0), true );
    float dScaleFactor = m_fcCue.params()->m_dScaleFactor;
    if( m_vPoints[i].scale > 1.0 )
      if( m_vPoints[i].l1 != m_vPoints[i].l2 )
        rsResultImg->drawEllipse( m_vPoints[i].x, m_vPoints[i].y,
                                  m_vPoints[i].l1*dScaleFactor, 
                                  m_vPoints[i].l2*dScaleFactor,
                                  -m_vPoints[i].angle,
                                  QColor::QColor(255,255,0), true );
    else
      rsResultImg->drawCircle( m_vPoints[i].x, m_vPoints[i].y,
                               m_vPoints[i].scale*3.0,
                               QColor::QColor(255,255,0), true );
  }
  qApp->processEvents();
}

vector<OpGrayImage> FeatExtract::getImagePatches()
{
	vector<OpGrayImage> vGrayImg;
	for(vector<PatchSample>::iterator itr = m_vpsPatch4Train.begin(); itr!=m_vpsPatch4Train.end();itr++){
		vGrayImg.push_back(itr->vChannels[0]);
	}
	return vGrayImg;
}

	
void FeatExtract::displayPatchesForBrowsing(vector<OpGrayImage> &vPatches)
/*******************************************************************/
/* Display the extracted image patches in a separate window.       */
/*******************************************************************/
{
  if ( vPatches.empty() )
    return;
  vector<QImage> vPatchImages;
  
  for( int i=0; i <(int)vPatches.size(); i++) {
    vPatchImages.push_back( vPatches[i].getQtImage() );
  }

  QtImgBrowser *qtPatchBrowser = new QtImgBrowser( 0, "patches");
  qtPatchBrowser->setGeometry( 1150, 200, 100, 150 );
  qtPatchBrowser->load( vPatchImages );
  qtPatchBrowser->show();
}

FeatExtract::~FeatExtract()
{
	if(vsHough!=NULL)
       delete vsHough;
	vsHough = NULL;
	if(m_pFeatures!=NULL)
		delete[] m_pFeatures;
	m_pFeatures = NULL;
	if(m_pClusterer!=NULL)
		delete[] m_pClusterer;
	m_pClusterer = NULL;
}

QStringList FeatExtract::loadFileSet(const QString& filenameinstructor, const QString& dir)
{
	QString lastDir = dir;
	QStringList qslFileList;
	QStringList qslFileListSeveral;
 	qslFileListSeveral = QFileDialog::getOpenFileNames(
                           filenameinstructor,
                           lastDir, this, "FileSet", "Select Files" );
  
  if (qslFileListSeveral.isEmpty())
    return qslFileListSeveral; 
  
  lastDir = qslFileListSeveral[0];
  cout << "  ...'" << qslFileListSeveral[0].toStdString() << "'... [" 
       << qslFileListSeveral.size() << "]" << endl;
  
  /* add entries for the image class */
  int nCount = (int)qslFileListSeveral.size();
  bool ok = true;
  while( ok ) {
    // ask for more images
    switch( QMessageBox::information( this, STR_NAME.c_str(),
                                      "Load more files?",
                                      "&Yes", "&No", "&Cancel",
                                      0, 2 ) ) {
    case 0: // Yes clicked
      qslFileList = QFileDialog::getOpenFileNames(
                       filenameinstructor,
                       lastDir, this, "FileSet", "Select More Files" );
      if ( !qslFileList.isEmpty() ) {
        lastDir = qslFileList[0];
        qslFileListSeveral += qslFileList; //Append
      } else
        ok = false;
      break;
      
    case 1: // No clicked
      ok = false;
      break;
      
    case 2: // Cancel clicked
      ok = false;
      return qslFileListSeveral;
      break;
    }
  }
  return qslFileListSeveral;
}

string FeatExtract::replacesuffix(const string& input, const string& toreplace)
{
	size_t suffixpos = input.rfind(".");
	string filename = input;
	filename = filename.replace(suffixpos, string::npos, toreplace);
	return filename;
}
