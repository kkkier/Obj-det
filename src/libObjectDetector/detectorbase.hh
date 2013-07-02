/* */

class virtual ObjectDetectProcedureBase
{
	virtual void loadImage();
	virtual void detect();
	virtual void postProcessing();
};

class ObjectDetectorBase
{
	ObjectDetectorBase();
	~ObjectDetectorBase();
	virtual void trainDetector();
	virtual void detectWithDetector();
};
