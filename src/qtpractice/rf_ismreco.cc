
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
#include <fstream>
#include <sstream>
#include <QWidget>
#include <QtGui>
#include <qtimgbrowser.hh>
#include "rf_ismreco.hh"
/****************/
/*	Defintions	*/
/***************/
RFISMReco::RFISMReco(QWidget *parent, const char *name):QWidget( parent)
{	
  vsHough = NULL;
	/*----------------------*/
	/*	Making the Title	*/
	/*----------------------*/
  QFont ft;
  ft.setPointSize(20);
  QLabel *logo = new QLabel(	(STR_NAME + " " + STR_VERSION).c_str(),
				this);
  logo->setFont(ft);
  /*----------------------------------------------------*/
  /*	The parameters setting for Random Forest		*/
  /*----------------------------------------------------*/
  QVBoxLayout	*vbRandomForest = new QVBoxLayout;
  QLabel		*lbRandomForest = new QLabel( "<b>1. Set the Random Forest Params</b>" );
  vbRandomForest->addWidget( lbRandomForest );
  QGroupBox		*gbRandomForest = new QGroupBox;
  gbRandomForest->setFixedSize( 300, 150 );
  RandomForestGUI *guiRandomForest = m_rfForest.createGUI( gbRandomForest );
  vbRandomForest->addWidget( gbRandomForest );  
  /*--------------------------------------------*/
  /*	Make a patch sampling setting group		*/
  /*--------------------------------------------*/
	//TODO: Implement the patch extraction GUI
	QVBoxLayout *vbfcCue = new QVBoxLayout;
	QGroupBox *gbfcCue = new QGroupBox();
	QLabel    *lbfcCue = new QLabel("<b>2. Set the Patch Sampling Params</b>");
	vbfcCue->addWidget(lbfcCue);
	vbfcCue->addWidget(gbfcCue);
	FeatureGUI *guiCue = m_fcCue.createGUI( gbfcCue, "cue1");
	
  /*--------------------------------------------*/
  /*	Make a codebook display(Ref. Leibe)		*/
  /*--------------------------------------------*/
  // 
  QGroupBox *gbIconBox = new QGroupBox;
  gbIconBox->setTitle("Leaf Nodes");
  //TODO: Implement the codebook display gui
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
  /*	Make the Training&Testing Button	*/
  /*----------------------------------------*/
	QVBoxLayout *vbTrainnTest = 		new QVBoxLayout;
	
	QLabel		*lbTrain = 				new QLabel("<b>3. Train</b>");
	QHBoxLayout	*hbTrain = 				new QHBoxLayout;
	QPushButton *btLoadImage4Train = 	new QPushButton("Load Image For Training");
	QPushButton *btTrain	= 			new QPushButton("Train");
	QPushButton *btTrainWithPuri = 		new QPushButton("Train with Puri.");
	hbTrain->addWidget(btLoadImage4Train);
	hbTrain->addWidget(btTrain);
	hbTrain->addWidget(btTrainWithPuri);

	QHBoxLayout *hbInvestigateLeafNode= new QHBoxLayout;
	QPushButton *btRankLeafNodes =      new QPushButton("Rank Leaf Nodes");
	QPushButton *btShowDisciminateLeafNodeCoverage = new QPushButton("Show Discriminative Node Coverage");
	hbInvestigateLeafNode->addWidget(btRankLeafNodes);
	hbInvestigateLeafNode->addWidget(btShowDisciminateLeafNodeCoverage);
	
	QHBoxLayout *hbSave = 				new QHBoxLayout;
	QPushButton *btSaveRandomForest=	new QPushButton("Save RandomForest");
	QPushButton *btLoadRandomForest=	new QPushButton("Load RandomForest");
	hbSave->addWidget(btSaveRandomForest);
	hbSave->addWidget(btLoadRandomForest);

	QLabel		*lbTest  = new QLabel("<b>4. Test</b>");
	QHBoxLayout	*hbTest  = new QHBoxLayout;
	QPushButton *btLoadImage4Test	= new QPushButton("Load Image For Testing");
	QPushButton *btTest			= new QPushButton("Test");
	hbTest->addWidget(btLoadImage4Test);
	hbTest->addWidget(btTest);
    
	QHBoxLayout *hbSetTest = new QHBoxLayout;
	QPushButton *btLoadImageSet4Test= new QPushButton("Load Image Set For Testing");
	hbSetTest->addWidget(btLoadImageSet4Test);
	
	vbTrainnTest->addWidget(lbTrain);
	vbTrainnTest->addLayout(hbTrain);
	vbTrainnTest->addLayout(hbSave);
	vbTrainnTest->addLayout(hbInvestigateLeafNode);
	vbTrainnTest->addWidget(lbTest);
	vbTrainnTest->addLayout(hbTest);
	vbTrainnTest->addLayout(hbSetTest);

	QGroupBox *blankBox = new QGroupBox;
	vbTrainnTest->addWidget(blankBox);
	//vbTrainnTest->setStretch(4,10);
	// make connections
	connect( btLoadImage4Train, SIGNAL( clicked() ),
			 this, SLOT( loadImageSet4Training()) );
	connect( btLoadImage4Test, SIGNAL( clicked()) ,
			 this, SLOT( loadImage4Testing() ));
	connect( btLoadImageSet4Test, SIGNAL( clicked()) ,
			 this, SLOT( loadImageSet4Testing() ));
	connect( btSaveRandomForest, SIGNAL( clicked()),
			 this, SLOT( saveRandomForest() ));
	connect( btLoadRandomForest, SIGNAL( clicked()),
			 this, SLOT( loadRandomForest()));
	connect( btTrain, SIGNAL( clicked() ),
			 this, SLOT( train() ) );
	connect( btTrainWithPuri, SIGNAL(clicked()),
			 this, SLOT( trainwithpuri() ) );
	connect( btRankLeafNodes, SIGNAL(clicked()),
			 this, SLOT( rankLeafNodes()));
	connect( btShowDisciminateLeafNodeCoverage, SIGNAL(clicked()),
			 this, SLOT( showDiscriminateLeafNodeCoverage()));
	connect( btTest, SIGNAL( clicked()),
			 this, SLOT( test() ) );
  /*----------------------------*/
  /*	Set the Global grid		*/
  /*----------------------------*/
	QGridLayout	*grid = new QGridLayout();
	grid->addWidget( logo, 0, 0, Qt::AlignHCenter);
	grid->addLayout( vbRandomForest, 1, 0 );
	grid->addLayout( vbShow, 2, 1 );
	grid->addLayout( vbTrainnTest, 1, 1 );
	//grid->addWidget( boxImg, 1, 1);
	//grid->addWidget( gbIconBox, 2, 1);
	gbfcCue->setFixedSize( 300,600 );
	grid->addLayout( vbfcCue, 2, 0);
	//grid->addLayout( hbIconBox, 2, 1);
	//grid->addLayout( hbImgBox, 1,0);
	this->setLayout(grid);
	//grid->setColumnStretch(1, 10);
	grid->addWidget(blankBox, 3,0);
	grid->setRowStretch(3, 10);
	//grid->setColumnStretch(2, 10);	
	rsSourceImg->loadImage( IMG_DIR+"/aeroplane/2008_000033.png");
	rsSourceImg->display();
	rsResultImg->loadImage( IMG_DIR+"/aeroplane/2008_000037.png");
	rsResultImg->display();	
   
    //Test the icon view 
    QImage img(IMG_DIR+"/aeroplane/2008_000033.png");
	qClassView->addImageThumbnail(img,"The first","The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
	qClassView->addImageThumbnail(img, "The first", "The first");
  /**********************************************/
  /*	Parameter fields for Random Forest	*/
  /**********************************************/
  /*tabpRandForest->addSpacing( 5 );
  QButtonGroup *bgRandForest = new QButtonGroup("Parameters:", tabwRandForest, "bgRandForest");
  bgRandForest->setColumnLayout( 2, Qt::Horizontal );
  
  QLabel	*labTreeNum = new QLabel("# Trees:", bgRandForest);
  QLineEdit	*editTreeNum = new QLineEdit("5" , bgRandForest, "edTreeNum");
  QLabel 	*labTreeDepth = new QLabel("Tree Depth:", bgRandForest);
  QLineEdit	*editTreeDepth = new QLineEdit("10", bgRandForest, "edTreeDep");
  QLabel	*labWeekClas	= new QLabel("Week Classifers per node", bgRandForest);
  QLineEdit	*editWeekClas	= new QLineEdit("50", bgRandForest, "edWeekClas");
  QLabel	*labMinSample	= new QLabel("Min. Sample", bgRandForest);
  QLineEdit	*editMinSample	= new QLineEdit("20", bgRandForest, "edMinSample");
  
  editTreeNum->setMaximumWidth(50);
  editTreeDepth->setMaximumWidth(50);
  editWeekClas->setMaximumWidth(50);
  editMinSample->setMaximumWidth(50);
  
  m_nTreeNumber = atoi(editTreeNum->text());
  m_nTreeDepth 	= atoi(editTreeDepth->text());
  m_nWeekClassifier = atoi(editWeekClas->text());
  m_nMinSample = atoi(editMinSample->text());
 
  QT_CONNECT_LINEEDIT(editTreeNum, TreeNum);
  QT_CONNECT_LINEEDIT(editTreeDepth, TreeDepth);
  QT_CONNECT_LINEEDIT(editWeekClas, WeekClassifier);
  QT_CONNECT_LINEEDIT(editMinSample, MinSample);

  tabpRandForest->addWidget(bgRandForest);*/
}

void RFISMReco::loadImageSet4Training()
{
  vector<int> vImageClass;
  m_vpsPatch4Train.clear();
 
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
  // Class 0 is reserved for the back ground class
  int nClassId = 1;
  int nCount = (int)qslFileListSeveral.size();
  for( int i=0; i<nCount; i++ )
    vImageClass.push_back ( nClassId );

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
        
        /* add entries for the image class */
        nClassId++;
        int nAdded = (int)qslFileList.size();
        for( int i=0; i<nAdded; i++ )
          vImageClass.push_back ( nClassId );
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
  m_vImageClass = vImageClass;
  m_qslFileList = qslFileListSeveral;

  for(unsigned i=0; i < qslFileListSeveral.count(); i++) {
    cout<<" Processing the "<< i <<"of "<<qslFileListSeveral.count()<<"images "<<endl;
    processImage( qslFileListSeveral[i], vImageClass[i], i);
  } 
  vector<OpGrayImage> vImagePatches = getImagePatches();
  displayPatchesForBrowsing( vImagePatches ); 
}

void RFISMReco::loadImage4Testing()
/*========================================================================*/
/* Load an image to test. First extract the patches,					  */
/* fill in PatchSample.                                                   */
/*========================================================================*/
{
  QString lastDir( IMG_DIR );
  m_vpsPatch4Test.clear();
  QStringList qslFileList;	
  qslFileList = QFileDialog::getOpenFileNames(
                           "Images (*.png *.ppm *.pgm *.jpg);;All files (*.*)",
                           lastDir, this, "imageset", "Select Images" );
  
  if (qslFileList.isEmpty())
    return;
  // TODO: handling several images
  processImage4Test(qslFileList[0]);

}

void RFISMReco::loadImageSet4Testing()
/*=========================================================================*/
/* Load a set of images for testing. For each image, extact patches and    */
/* do testing for each class.                                              */
/*=========================================================================*/
{	
  /*-------------------------*/
  /*   Load the image sets   */
  /*-------------------------*/
  vector<int> vImageClass;
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
  int nClassId = 1;
  int nCount = (int)qslFileListSeveral.size();
  for( int i=0; i<nCount; i++ )
    vImageClass.push_back ( nClassId );

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
        
        /* add entries for the image class */
        nClassId++;
        int nAdded = (int)qslFileList.size();
        for( int i=0; i<nAdded; i++ )
          vImageClass.push_back ( nClassId );
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
    /*--------------------------*/
	/* Prepare the result files */
	/*--------------------------*/
  fstream fsResult; 
  fstream fsGround;	
  fsResult.open( "result.txt", fstream::in | fstream::out | fstream::trunc );
  fsGround.open( "ground.txt", fstream::in | fstream::out | fstream::trunc );
  fsResult<< m_vpsPatch4Train[0].nTotalClasses<<endl;
  fsGround<< m_vpsPatch4Train[0].nTotalClasses<<endl;
  for(unsigned i=0; i < qslFileListSeveral.count(); i++) {
	/*--------------------------*/
	/*     Process the image    */
    /*--------------------------*/
	m_vpsPatch4Test.clear();
    cout<<" Processing the "<< i <<"of "<<qslFileListSeveral.count()<<"images "<<endl;
    processImage4Test( qslFileListSeveral[i]);
	m_nImgClass = vImageClass[i]; 
	/*--------------------------*/
	/*     Test the image       */
	/*--------------------------*/
	vector<float> vdValue;
	vector<FeatureVector> vfvCoord;
	test( vfvCoord, vdValue);
	/*--------------------------*/
	/*     Write the result     */
	/*--------------------------*/
	for(int j=0; j < vdValue.size(); j++)
    {
		fsResult<<vfvCoord[j].at(0)<<" "<<vfvCoord[j].at(1)<<endl;
		fsResult<<vdValue[j]<<endl;
	}
    fsResult<<endl;
	int center_x,center_y;
	m_grayImgMap.opComputeCoG(center_x, center_y);
	fsGround<<center_x<<" "<<center_y<<endl;
	fsGround<<vImageClass[i]<<endl;
  } 
  fsResult.close();
  fsGround.close();
}


void RFISMReco::train()
{
	if(m_vpsPatch4Train.size()==0){
		cerr<<"The patches for training have not been collected yet!";
		return;
	}
	m_rfForest.trainRandomForest(m_vpsPatch4Train);

    //Display the trained nodes
	m_rfForest.drawClusters( qClassView );	
	//Write the leaf node info to file
	//QString fileName = QFileDialog::getSaveFileName(this, tr("Leaf Node File"), tr("~/code/qtpractice/"));
	//m_rfForest.writeLeafNodetoFile(fileName);
	//m_rfForest.saveRandomForest(fileName.toStdString().data());
}

void RFISMReco::trainwithpuri()
/*==================================================================*/
/*  Train the random forest with training sample purification 		*/
/*  procedure.														*/
/*==================================================================*/
{
	if(m_vpsPatch4Train.size()==0)
	{
		cerr<<"The patches for training have not been collected yet!";
		return;
	}
	vector<uint32_t> removedIndex;
	vector<uint32_t> top100DiscriminantIndex;
	m_rfForest.trainRandomForest(m_vpsPatch4Train);
	m_rfForest.drawClusters(qClassView);
	m_rfForest.purifyTrainingSet(removedIndex, top100DiscriminantIndex);
	//Display the removed patches
	vector<OpGrayImage> vImg;
	vector<QImage> vQImg;
	for(int i=0;i<removedIndex.size();i++)
	{
		vImg.push_back(m_vpsPatch4Train[removedIndex[i]].vChannels[0]);
		vQImg.push_back(m_vpsPatch4Train[removedIndex[i]].vChannels[0].getQtImage());
	}
	QtImgBrowser *qtSupportingBrowser = new QtImgBrowser(0, "Removed positive patches");
	qtSupportingBrowser->setGeometry(950, 200, 300, 350);
	qtSupportingBrowser->load(vQImg, vImg);
	qtSupportingBrowser->show();

	// Display the most discriminant patches
	// They the just the examples of the discriminant patches
	vImg.clear();
	vQImg.clear();
	for(int i=0;i<top100DiscriminantIndex.size();i++)
	{
		vImg.push_back(m_vpsPatch4Train[top100DiscriminantIndex[i]].vChannels[0]);
		vQImg.push_back(m_vpsPatch4Train[top100DiscriminantIndex[i]].vChannels[0].getQtImage());
	}
	QtImgBrowser *qtSupportingBrowser2 = new QtImgBrowser(0, "The most discriminant patches");
	qtSupportingBrowser2->setGeometry(950, 200, 300, 350);
	qtSupportingBrowser2->load(vQImg, vImg);
	qtSupportingBrowser2->show();

}

void RFISMReco::rankLeafNodes()
{
	vector<LeafNodeScore> leafnodescore = m_rfForest.rankLeafNodes();
	showRankedLeafNodes(leafnodescore);
}

void RFISMReco::showDiscriminateLeafNodeCoverage()
{
	/*vector< vector<Tuple_Sample_LeafNodeIndex> > leafnodeperimage = m_rfForest.getTwoLeafNodeCoveringofImages(m_qslFileList.size());
	vector<QImage> vQImg;
	vector<OpGrayImage> vImg;
	vImg = drawLeafNodesOnImages(leafnodeperimage);
	vQImg = grayImageVector2QImageVector(vImg);
	QtImgBrowser *qtLeafNodeCoverageBrowser = new QtImgBrowser(0, "Discriminative Leaf Node Coverage");
	qtLeafNodeCoverageBrowser->setGeometry(950, 200, 300, 350);
	qtLeafNodeCoverageBrowser->load(vQImg, vImg);
	qtLeafNodeCoverageBrowser->show();

	vector< vector<Tuple_Sample_LeafNodeIndex> > positiveleafnodeperimage = m_rfForest.getTwoLeafNodeCoveringofImages(m_qslFileList.size());
	vQImg.clear();
	vImg.clear();
	vImg = drawLeafNodesOnImages(positiveleafnodeperimage);
	vQImg = grayImageVector2QImageVector(vImg);
	QtImgBrowser *qtPositiveLeafNodeCoverageBrowser = new QtImgBrowser(0, "Positive Discriminative Leaf Node Coverage");
	qtPositiveLeafNodeCoverageBrowser->setGeometry(950, 200, 300, 350);
	qtPositiveLeafNodeCoverageBrowser->load(vQImg, vImg);
	qtPositiveLeafNodeCoverageBrowser->show();*/
	
	vector< vector<Tuple_Sample_LeafNodeIndex> > leafnodeperimage = m_rfForest.getAverage5LeafNodePerImageCoverage(m_qslFileList.size());
	vector<QImage> vQImg;
	vector<OpGrayImage> vImg;
	vImg = drawLeafNodesOnImages(leafnodeperimage);
	vQImg = grayImageVector2QImageVector(vImg);
	QtImgBrowser *qtLeafNodeCoverageBrowser = new QtImgBrowser(0, "Discriminative Leaf Node Coverage");
	qtLeafNodeCoverageBrowser->setGeometry(950, 200, 300, 350);
	qtLeafNodeCoverageBrowser->load(vQImg, vImg);
	qtLeafNodeCoverageBrowser->show();
}

vector<OpGrayImage> RFISMReco::drawLeafNodesOnImages(vector< vector<Tuple_Sample_LeafNodeIndex> >leafnodeperimage)
{
	vector<OpGrayImage> resultImages;
	for(uint32_t i = 0; i < leafnodeperimage.size(); i++)
	{
		vector<Tuple_Sample_LeafNodeIndex> leafnodelist = leafnodeperimage[i];
		loadImage(m_qslFileList[i]);	
		OpGrayImage pallete = m_grayImg;
		pallete = drawSeveralLeafNodesOnOneImage(pallete, leafnodelist);
		resultImages.push_back(pallete);
	}
	return resultImages;
}

vector<QImage> RFISMReco::grayImageVector2QImageVector(const vector<OpGrayImage>& grayImageVector)
{
	vector<QImage> vQImg;
	for(vector<OpGrayImage>::const_iterator itr = grayImageVector.begin(); itr != grayImageVector.end(); itr++)
	{
		vQImg.push_back(itr->getQtImage());
	}
	return vQImg;
}

OpGrayImage RFISMReco::drawSeveralLeafNodesOnOneImage(OpGrayImage inputImage, vector<Tuple_Sample_LeafNodeIndex> leafnodesonimage)
{
	OpGrayImage resultImage = inputImage;
	//TODO: add a lineedit to set the number to show
	for(int i = 0; (i < leafnodesonimage.size())&&(i < 10); i++)
	{
		resultImage = drawOneLeafNodeOnOneImage(resultImage, leafnodesonimage[i]);
	}
	return resultImage;
}

OpGrayImage RFISMReco::drawOneLeafNodeOnOneImage(OpGrayImage inputImage, Tuple_Sample_LeafNodeIndex leafnodeonimage)
{
	OpGrayImage resultImage = inputImage;
	InterestPoint offset;
	int center_x, center_y;
	m_grayImgMap.opComputeCoG(center_x, center_y);
	offset.x = center_x - m_vpsPatch4Train[leafnodeonimage.sampleindex].fvOffset.x;
	offset.y = center_y - m_vpsPatch4Train[leafnodeonimage.sampleindex].fvOffset.y;

	int tree = leafnodeonimage.leafnodeindex.tree;
	uint32_t key = leafnodeonimage.leafnodeindex.key;

	OpGrayImage leafnodepatch = m_rfForest.getMeanPatchOfLeafNode(tree, key);

	resultImage = drawImagePatchOnImage(leafnodepatch, resultImage, offset);

	return resultImage;
}

OpGrayImage RFISMReco::drawImagePatchOnImage(OpGrayImage imgPatch, OpGrayImage pallete, InterestPoint offset)
{
	OpGrayImage resultImage = pallete;
	int nWidth =  imgPatch.width();
	int nHeight =  imgPatch.height();
	int minxx = 0;
	int minyy = 0;
	int maxxx = pallete.width();
	int maxyy = pallete.height();
	int minx = max(offset.x - nWidth/2, 0);
	int maxx = min(offset.x + nWidth/2, maxxx);
	int miny = max(offset.y - nHeight/2, 0);
	int maxy = min(offset.y + nHeight/2, maxyy);
	for(int y=miny, yy=minyy; (y<maxy)&&(yy<maxyy); y++, yy++)
		for(int x=minx, xx=minxx; (x<maxx)&&(xx<maxxx); x++, xx++)
		{
			float dPatchValRaw = imgPatch(xx,yy).value();
			if(dPatchValRaw>=0)
				resultImage(x,y) = dPatchValRaw;
			if((y == maxy-1) || (yy == maxyy-1) || (x == maxx-1) || (xx == maxxx-1) 
			 ||(y == miny) || (yy == minyy) || (x == minx) || (xx == minxx))
			 	resultImage(x,y) = static_cast<float>(0);
		}
	
	return resultImage;
}

void RFISMReco::showRankedLeafNodes(const vector<LeafNodeScore>& leafnodescore)
{
	vector<OpGrayImage> vImg;
	vector<QImage> vQImg;
	for(uint32_t i=0; i<leafnodescore.size(); i++)
	{
		vImg.push_back(m_rfForest.getMeanPatchOfLeafNode(leafnodescore[i].index.tree, leafnodescore[i].index.key));
		vQImg.push_back(vImg.back().getQtImage());
	}
	QtImgBrowser *qtSupportingBrowser = new QtImgBrowser(0, "The most discriminant leaf Nodes");
	qtSupportingBrowser->setGeometry(950, 200, 300, 350);
	qtSupportingBrowser->load(vQImg, vImg);
	qtSupportingBrowser->show();
}

void RFISMReco::saveRandomForest()
/*===============================================*/
/* Save the trained randomforest to a .xml file. */
/*===============================================*/
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("RandomForest"), tr("~/code/qtpractice/"));
	m_rfForest.saveRandomForest(fileName.toStdString().data());
}

void RFISMReco::loadRandomForest()
/*===============================================*/
/* Load a randomforest for a .xml file.          */
/*===============================================*/
{
//	QString fileName =  QFileDialog::getOpenFileName(this, tr("Load RandomForest"), "~/code/qtpractice",  "XML (*.xml *.XML *.Xml);;All files (*.*)"); 
	QString fileName = QFileDialog::getOpenFileName(
                           "XML (*.xml *.XML *.Xml);;All files (*.*)",
                           "XML (*.xml *.XML *.Xml);;All files (*.*)", this, "imageset", "Select Images" );

	m_rfForest.loadRandomForest(fileName.toStdString().data());
}

void RFISMReco::test()
/*==================================================================*/
/*  Impl. function of the test button.Should be used after 			*/
/*  loadImage4Test. Build the voting space with 					*/
/*  m_vpsPatch4Test, display it, find hypotheses, and verify if nece*/
/* ssary.Only draws the result voting space and returns nothing.    */
/*==================================================================*/
{
  if(m_vpsPatch4Test.size()==0)
  {
	cout<<"Error in RFISMReco::test: no patches have been collected for testing."<<endl;
	exit(0);
  }
  vector<int> vBins(3);
  vBins[0] = m_grayImg.width()/DEFAULTSTEPSIZE;
  vBins[1] = m_grayImg.height()/DEFAULTSTEPSIZE;
  vBins[2] = DEFAULTMAXSCALE - DEFAULTMINSCALE;
  cout<<"Creating VotingSpace:"<<endl;
  cout<<" Object size:("<<DEFAULTOBJECTWIDTH<<","<<DEFAULTOBJECTHEIGHT<<")"<<endl;
  cout<<" Bins:("<<vBins[0]<<","<<vBins[1]<<","<<vBins[2]<<")"<<endl;
  if(vsHough!=NULL)
  {
	delete vsHough;
	vsHough = NULL;
  }
  vsHough = new VotingSpace(m_grayImg.width(), m_grayImg.height(),
                            DEFAULTOBJECTWIDTH, DEFAULTOBJECTHEIGHT,	
							DEFAULTMINSCALE, DEFAULTMAXSCALE,
							DEFAULTDIM, vBins,
							&m_rfForest); 
 for(int i=0;i<m_vpsPatch4Test.size();i++)
 {
	vsHough->voteWithPatch( m_vpsPatch4Test[i], i );
 }
 vsHough->displayConfMap();
 vector<FeatureVector> vfvCoord;
 vector<float> vdValue;
 vector<OpGrayImage> vSupportImg;
 FeatureVector fvResult;
 float dScore;
for(int nClass=1;nClass<m_rfForest.getTotalClasses();nClass++)
{
	float dValue = 0;
	FeatureVector fvCoord;
	for(int i = 0;i<10;i++)
	{
		// Generate random start point
		int x = random() % m_grayImg.width();
		int y = random() % m_grayImg.height();
		vsHough->applyMSME( x, y, nClass, fvResult, dScore);
		// Find the highest score
		if(dScore > dValue)
		{
			dValue = dScore;
			fvCoord = fvResult;
		}
	}
	// Retrieve the supporting votes for the highest scored position
	vector<HoughVote> vVotes;
	vector<int> vnSupportingIndex = vsHough->getSupportingPatches(nClass,
																static_cast<int>(floor(fvCoord.at(0))),
																static_cast<int>(floor(fvCoord.at(1))), 
																2,
																vVotes);
		// Store the supporting patches' information to the file(only those above the threshold)
		// The stored information includes:
		// 		Patch's position
		// 		The leaf nodes' voting offset(of all the random trees)
	fstream fsSupportingVoteFile;
	ostringstream osFileName;
	osFileName << "SupportingInfo_Class_" << nClass;
	fsSupportingVoteFile.open(osFileName.str().data(), fstream::in|fstream::out|fstream::trunc);
	for(int n=0; n<vVotes.size();n++)
	{
		// The patch's position
		fsSupportingVoteFile<< "Patch"
							<<endl;
		PatchSample psPatch = m_vpsPatch4Test[vVotes[n].m_nPatchIndex];
		fsSupportingVoteFile<<psPatch.fvOffset.x
							<<","
							<< psPatch.fvOffset.y
							<<endl;
		fsSupportingVoteFile<<"Offset"<<endl;
		// All the offset coordinates of the current patch
		for(int i = 0; i < vVotes[n].m_vnLeafNodes.size(); i++)
		{
			RandomNode node = m_rfForest.getNode(i,vVotes[n].m_vnLeafNodes[i]);
			for(int j=0; j<node.m_vCoords[nClass].size();j++)
			{
				fsSupportingVoteFile 	<< node.m_vCoords[nClass][j].at(0)
										<< ","
										<< node.m_vCoords[nClass][j].at(1)
										<<endl;
			}
		}
	}
	fsSupportingVoteFile.close();
	// prepare a black background
	float dBlackValue = 255.0;
	OpGrayImage imgSupportingResult(m_grayImg.width(), m_grayImg.height() );
	for(int y=0; y<m_grayImg.height();y++)
		for(int x=0; x<m_grayImg.width();x++)
			imgSupportingResult(x,y) = dBlackValue; 
	
	// draw the matched patches
	for(vector<int>::iterator itr = vnSupportingIndex.begin(); itr!=vnSupportingIndex.end(); itr++)
	{
		int nWidth =  m_vpsPatch4Test[0].vChannels[0].width();
		int nHeight =  m_vpsPatch4Test[0].vChannels[0].height();
		int minxx = 0;
		int minyy = 0;
		int maxxx = nWidth;
		int maxyy = nHeight;
		int minx = max(m_vpsPatch4Test[*itr].fvOffset.x - nWidth/2, 0);
		int maxx = min(m_vpsPatch4Test[*itr].fvOffset.x + nWidth/2, m_grayImg.width());
		int miny = max(m_vpsPatch4Test[*itr].fvOffset.y - nHeight/2, 0);
		int maxy = min(m_vpsPatch4Test[*itr].fvOffset.y + nHeight/2, m_grayImg.height());
		for(int y=miny, yy=minyy; (y<maxy)&&(yy<maxyy); y++, yy++)
			for(int x=minx, xx=minxx; (x<maxx)&&(xx<maxxx); x++, xx++)
			{
				float dPatchValRaw = m_vpsPatch4Test[*itr].vChannels[0](xx,yy).value();
				if(dPatchValRaw>=0)
					imgSupportingResult(x,y) = dPatchValRaw;
			}
	}
	vfvCoord.push_back( fvCoord );
	vdValue.push_back( dValue );
	vSupportImg.push_back( imgSupportingResult );
}
// Display the supporting image
	vector<QImage> vImgs;
	for(int i=0; i<(int)vSupportImg.size();i++)
	{
		vImgs.push_back(vSupportImg[i].getQtImage() );
	}
	QtImgBrowser *qtSupportingBrowser = new QtImgBrowser(0, "Supporting Votes");
	qtSupportingBrowser->setGeometry(950, 200, 300, 350);
	qtSupportingBrowser->load(vImgs, vSupportImg);
	qtSupportingBrowser->show();
}

void RFISMReco::test(	vector<FeatureVector>&				vfvCoord,
						vector<float>&						vdValue)
/*================================================================*/
/* Test the image patches which are derived from processImage. The*/
/* hypotheses are generated by applying mean-shift from several   */
/* randomly generated initial positions, and the result with the  */
/* highest value is returned.									  */
/*================================================================*/	
{
  if(m_vpsPatch4Test.size()==0)
  {
	cout<<"Error in RFISMReco::test: no patches have been collected for testing."<<endl;
	exit(0);
  }

  vector<int> vBins(3);
  vBins[0] = m_grayImg.width()/DEFAULTSTEPSIZE;
  vBins[1] = m_grayImg.height()/DEFAULTSTEPSIZE;
  vBins[2] = DEFAULTMAXSCALE - DEFAULTMINSCALE;
  cout<<"Creating VotingSpace:"<<endl;
  cout<<" Object size:("<<DEFAULTOBJECTWIDTH<<","<<DEFAULTOBJECTHEIGHT<<")"<<endl;
  cout<<" Bins:("<<vBins[0]<<","<<vBins[1]<<","<<vBins[2]<<")"<<endl;
  if(vsHough!=NULL)
  {
	delete vsHough;
	vsHough = NULL;
  }
  vsHough = new VotingSpace(m_grayImg.width(), m_grayImg.height(),
                            DEFAULTOBJECTWIDTH, DEFAULTOBJECTHEIGHT,	
							DEFAULTMINSCALE, DEFAULTMAXSCALE,
							DEFAULTDIM, vBins,
							&m_rfForest); 
 for(int i=0;i<m_vpsPatch4Test.size();i++)
 {
	vsHough->voteWithPatch( m_vpsPatch4Test[i], i );
 }
 FeatureVector fvResult;
 float dScore;
 vfvCoord.clear();
 vdValue.clear();
for(int nClass=1;nClass<m_rfForest.getTotalClasses();nClass++)
{
	float dValue = 0;
	FeatureVector fvCoord;
	for(int i = 0;i<10;i++)
	{
		// Generate random start point
		int x = random() % m_grayImg.width();
		int y = random() % m_grayImg.height();
		vsHough->applyMSME( x, y, nClass, fvResult, dScore);
		// Find the highest score
		if(dScore > dValue)
		{
			dValue = dScore;
			fvCoord = fvResult;
		}
	}

	vfvCoord.push_back( fvCoord );
	vdValue.push_back( dValue );
}
	delete vsHough;
	vsHough = NULL;
}
void RFISMReco::processImage4Test(QString qstr)
/*==================================================================*/
/* Load the image and extract patches and store in m_vpsPatch4Test  */
/*==================================================================*/
{
  loadImage( qstr );
  qApp->processEvents(); // finish drawing
  
  QImage qimg = m_grayImg.getQtImage();
  PointVector vPoints, vPointsInside;
  vector<OpGrayImage> vImagePatches;
  vector<FeatureVector> vFeatures;
  cout<<"   Start Processing image..."<<endl;
  m_fcCue.processImage( m_sImgFullName, m_grayImg, m_grayImgMap, qimg, 
                        m_nFeatureType, vPoints, vPointsInside, 
                        vImagePatches, vFeatures);
  
  /*--------------------------------*/
  /*	Store the interest points	*/
  /*--------------------------------*/
   m_vPoints       = vPoints;
   m_vPointsInside = vPointsInside;
	
  /*----------------------------------------------------*/
  /*	Add the patch to database: m_vpsPatch4Test      */
  /*----------------------------------------------------*/
	for(int i = 0;i < vImagePatches.size();i++){
		PatchSample psCurrentPatch;
		psCurrentPatch.vChannels.push_back(vImagePatches[i]);
		psCurrentPatch.nChannels = 1;
		// Including the background class
    	InterestPoint ipTmp = m_vPointsInside[i];
		// Offsets are the offsets in image.
		psCurrentPatch.fvOffset = ipTmp;
		m_vpsPatch4Test.push_back(psCurrentPatch);
		
	}
  /*----------------------*/
  /* Draw interest points */
  /*----------------------*/
  drawInterestPointsEllipse();
  qApp->processEvents();

}
void RFISMReco::processImage(QString qstr, int nImageClass, uint32_t nImageIndex)
/*=================================================================*/
/* Process a new image, i.e. load it and extract patches using the */
/* methods selected in the used interface. The extracted patches   */
/* are then stored in m_vpsPatch4Train.                            */
/*=================================================================*/
{
  loadImage( qstr );
  qApp->processEvents(); // finish drawing
  
  QImage qimg = m_grayImg.getQtImage();
  PointVector vPoints, vPointsInside;
  vector<OpGrayImage> vImagePatches;
  vector<FeatureVector> vFeatures;
  cout<<"   Start Processing image..."<<endl;
  m_fcCue.processImage( m_sImgFullName, m_grayImg, m_grayImgMap, qimg, 
                        m_nFeatureType, vPoints, vPointsInside, 
                        vImagePatches, vFeatures);
  
  /*--------------------------------*/
  /*	Store the interest points	*/
  /*--------------------------------*/
   m_vPoints       = vPoints;
   m_vPointsInside = vPointsInside;
	
  /*----------------------------------------------------*/
  /*	Add the patch to database: m_vpsPatch4Train		*/
  /*----------------------------------------------------*/
	int center_x, center_y;
	m_grayImgMap.opComputeCoG(center_x, center_y);
	for(int i = 0;i < vImagePatches.size();i++){
		PatchSample psCurrentPatch;
		psCurrentPatch.vChannels.push_back(vImagePatches[i]);
		psCurrentPatch.nChannels = 1;
		// Including the background class
		psCurrentPatch.nTotalClasses = m_vImageClass.back() + 1;
    	InterestPoint ipTmp = m_vPointsInside[i];
		// Judge whether the patch is a background patch
		if(ipTmp.value>0)
			psCurrentPatch.nClassLabel = nImageClass;
		else
		    psCurrentPatch.nClassLabel = 0;
		ipTmp.x = center_x - ipTmp.x;
		ipTmp.y = center_y - ipTmp.y;
		psCurrentPatch.fvOffset = ipTmp;
		psCurrentPatch.nImageIndex = nImageIndex;
		m_vpsPatch4Train.push_back(psCurrentPatch);
		
	}
  /*----------------------*/
  /* Draw interest points */
  /*----------------------*/
  drawInterestPointsEllipse();
  qApp->processEvents();
}

void RFISMReco::loadImage(QString qstr)
/*=================================================================*/
/* Load a new test image (including its segmentation mask, if one  */
/* exists). Specifically, the image and mask are stored in  class  */
/* member, and correspondig directories strings are stored.        */
/*=================================================================*/
{
  cout << "  Clusterer::loadImage() called." << endl;
  m_img.load( qstr );
  
  /* if possible load the corresponding segmentation maps */
  QString nameOfMap( qstr );
  int posDir = nameOfMap.findRev( '/' );
  QString currentDir = nameOfMap.left( posDir+1 );
  nameOfMap = nameOfMap.right( nameOfMap.length() - posDir - 1 );
  QString mapDir("maps/");
  currentDir.append( mapDir );
  posDir = nameOfMap.findRev( '.' );
  
  if ( posDir >= 0 )
    nameOfMap = nameOfMap.left( posDir );
  QString fend("-map");
  nameOfMap.append( fend );
  QString ftype(".png");
  nameOfMap.append( ftype );
  
  currentDir.append( nameOfMap );
  QFile mapfile( currentDir );
  if ( mapfile.exists() == false ) {
    cout << "No corresponding segmentation map found." << endl;
    m_bMapsOn = false;

  } else {
    cout << "Corresponding segmentation map found." << endl;
    m_bMapsOn = true;
	QImage imgMap;
    imgMap.load( currentDir );
    OpGrayImage grayImgMap( imgMap );
    m_grayImgMap = grayImgMap;
  }
  
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
  
  if ( m_bMapsOn ) {
    /* show the segmentation map instead of the gray scale image */
    QImage timg = m_grayImgMap.getQtImage();
    rsResultImg->loadImage( timg, m_grayImgMap );
  }
  else {
    rsResultImg->loadImage( m_img, grayImg );
  }
  
  rsResultImg->display();
  
  m_bPicLoaded = true;
  
  cout << "  done." << endl;
}

void RFISMReco::drawInterestPoints()
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

void RFISMReco::drawInterestPointsEllipse()
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

vector<OpGrayImage> RFISMReco::getImagePatches()
{
	vector<OpGrayImage> vGrayImg;
	for(vector<PatchSample>::iterator itr = m_vpsPatch4Train.begin(); itr!=m_vpsPatch4Train.end();itr++){
		vGrayImg.push_back(itr->vChannels[0]);
	}
	return vGrayImg;
}

	
void RFISMReco::displayPatchesForBrowsing(vector<OpGrayImage> &vPatches)
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

RFISMReco::~RFISMReco()
{
	if(vsHough!=NULL)
       delete vsHough;
	vsHough = NULL;
}


