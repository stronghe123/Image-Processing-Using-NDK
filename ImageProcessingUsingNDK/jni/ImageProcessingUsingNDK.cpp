#include <jni.h>
#include <stdlib.h>
#include <stdio.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <coffeecatch.h>

#define APP_NAME 	"ImageProcessingUsingNDK"
#define LOG_E(...) 	__android_log_print(ANDROID_LOG_ERROR,APP_NAME,__VA_ARGS__)

/**
 * structure defined for the colors (ARGB)
 */
typedef struct {
	uint8_t alpha;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
}argb;

//#ifdef __cplusplus
extern "C" {
//#endif

/**
 * performs operations on the image to convert the image to gray
 */
void convertingToGray(void * pixelsColor, void * pixelsGrayColor,
		AndroidBitmapInfo * infoColor, AndroidBitmapInfo * grayImageInfo) {
	int y,x;

	for (y=0;y<infoColor->height;y++) {
		argb * line = (argb *) pixelsColor;
		
		// here we use uint_8 instead of uint32_t since gray images are alpha_8 images
		// that use 8 bit coloring system and not 32
		uint8_t * grayline = (uint8_t *) pixelsGrayColor;

		for (x=0;x<infoColor->width;x++) {
			//grayline[x] = 0.21 * line[x].red + 0.71 * line[x].green + 0.07 * line[x].blue;
			grayline[x] = 0.3 * line[x].red + 0.59 * line[x].green + 0.11*line[x].blue;
		}

		pixelsColor = (char *)pixelsColor + infoColor->stride;
		pixelsGrayColor = (char *) pixelsGrayColor + grayImageInfo->stride;
	}
}

/**
 * Performs operations to convert an image into red scale
 */
void convertingImageToRed(AndroidBitmapInfo * infoColor, void * pixelsColor,
		void * pixelsRedColor, AndroidBitmapInfo * redImageInfo) {
	int y,x;

	for (y=0;y<infoColor->height;y++) {
		argb * greyline = (argb *) pixelsColor;
		argb * sepialine = (argb *) pixelsRedColor;

		for (x=0;x<infoColor->width;x++) {
			sepialine[x].red =  0.1 *greyline[x].red;
			sepialine[x].green = 0.9;
			sepialine[x].blue = 0;
			sepialine[x].alpha = greyline[x].alpha;
		}

		pixelsColor = (char *)pixelsColor + infoColor->stride;
		pixelsRedColor = (char *) pixelsRedColor + redImageInfo->stride;
	}
}

/**
 * Performs operations on the image to warmify it
 */
void warmifyingImage(AndroidBitmapInfo * infoColor, void * pixelsColor,
		void * pixelsWarmColor , AndroidBitmapInfo * warmImageColor) {
	int y,x;

	for (y=0;y<infoColor->height;y++) {
		argb * line = (argb *) pixelsColor;
		argb * warmline = (argb *) pixelsWarmColor;

		/* 	greenline[x].red = 0 * line[x].red + 0.5* line[x].green + 0 * line[x].blue;
			greenline[x].green = 0.803 * line[x].green + 0.08 * line[x].blue; //0.009 * line[x].red + 0.803 * line[x].green + 0.008 * line[x].blue;
			greenline[x].blue =  0.0;
			greenline[x].alpha = line[x].alpha;

			greenline[x].red = 0.5* line[x].red;// 0 * line[x].red + 0.02* line[x].green + 0 * line[x].blue;
			greenline[x].green = line[x].green;//0 * line[x].red + 1.0 * line[x].green + 0.0 * line[x].blue; //0.009 * line[x].red + 0.803 * line[x].green + 0.008 * line[x].blue;
			greenline[x].blue =0* line[x].blue;
			greenline[x].alpha = line[x].alpha;
		 */
		for (x=0;x<infoColor->width;x++) {
			// 0 * line[x].red + 0.02* line[x].green + 0 * line[x].blue;
			warmline[x].red = 0.65 * line[x].red;

			//0 * line[x].red + 1.0 * line[x].green + 0.0 * line[x].blue; //0.009 * line[x].red + 0.803 * line[x].green + 0.008 * line[x].blue;
			warmline[x].green = 1.098 * line[x].green;

			//1.1 * line[x].blue;
			warmline[x].blue =0;

			warmline[x].alpha = line[x].alpha;
		}

		pixelsColor = (char *)pixelsColor + infoColor->stride;
		pixelsWarmColor = (char *) pixelsWarmColor + warmImageColor->stride;
	}
}

/**
 * Image conversion code to sepia (almost)
 */
void convertingImageToSepia(AndroidBitmapInfo * infoColor, void *  pixelsColor,
		void * pixelsSepia, AndroidBitmapInfo * sepiaImageColor) {

	/*  =====================================	*/
	/*  For refrerence, SEPIA CODE
		R' = (R � 0.393 + G � 0.769 + B � 0.189);
		G' = (R � 0.349 + G � 0.686 + B � 0.168);
		B' = (R � 0.272 + G � 0.534 + B � 0.131);
	/* 	===================================== 	*/

	int y,x;

	for (y=0;y<infoColor->height;y++) {
		argb * line = (argb *) pixelsColor;
		argb * sepialine = (argb *) pixelsSepia;

		/* sepialine[x].red = 0.293 * line[x].red + 0.169* line[x].green + 0.489 * line[x].blue;
				sepialine[x].green = 0.349 * line[x].red + 0.483 * line[x].green + 0.168 * line[x].blue;
				sepialine[x].blue =  0.292 * line[x].red + 0.554 * line[x].green + 0.154 * line[x].blue;
				sepialine[x].alpha = line[x].alpha; */

		for (x=0;x<infoColor->width;x++) {
			// 0.393 * line[x].red + 0.369* line[x].green + 0.189 * line[x].blue;
			/*sepialine[x].red = 0.003 * line[x].red + 0.549* line[x].green + 0.449 * line[x].blue;
					sepialine[x].green = 0.009 * line[x].red + 0.903 * line[x].green + 0.008 * line[x].blue;
					sepialine[x].blue =  0.004 * line[x].red + 0.254 * line[x].green + 0.150 * line[x].blue;
					sepialine[x].alpha = line[x].alpha;*/

			sepialine[x].red = 0.393 * line[x].red + 0.369* line[x].green + 0.189 * line[x].blue;
			sepialine[x].green = 0.349 * line[x].red + 0.483 * line[x].green + 0.168 * line[x].blue;
			sepialine[x].blue =  0.292 * line[x].red + 0.554 * line[x].green + 0.154 * line[x].blue;
			sepialine[x].alpha = line[x].alpha;
		}

		pixelsColor = (char *)pixelsColor + infoColor->stride;
		pixelsSepia = (char *) pixelsSepia + sepiaImageColor->stride;
	}
}

int truncate(int value) {
	if(value < 0) {
		return 0;
	}

	if(value > 255) {
		return 255;
	}

	return value;
}

/**
 * Increases the brightness of an image based on the value received.
 * The values could be between 1 and 10
 */
void increasingBrightnessOfImage(AndroidBitmapInfo * infogray, void* pixelsgray,
		float brightnessValue) {

	// For reference :  http://www.easyrgb.com/index.php?X=MATH
	//					(0.2126*R) + (0.7152*G) + (0.0722*B) -> Luminance

	int red, green,blue;
	int y,x;

	for (y=0;y<infogray->height;y++) {
		//uint8_t * line = (uint8_t *)pixelsgray;
		
		// commenting the line above since 1/4 th of the image gets converted
		// and not the whole, ARGB = 32 bits color system, while
		// uint8_t uses 8bits.(convers 8 bits)
		
		uint32_t * line = (uint32_t *)pixelsgray;
		for (x=0;x<infogray->width;x++) {

			red = (int)((line[x] & 0x00FF0000) >> 16);
			green = (int)((line[x] & 0x0000FF00) >> 8);
			blue = (int)((line[x] & 0x000000FF));

			red = truncate((int)(red * brightnessValue));
			green = truncate((int)(green * brightnessValue));
			blue= truncate((int)(blue * brightnessValue));

			line[x] = ((red << 16) & 0x00FF0000) |
					((green << 8) & 0x0000FF00) |
					(blue & 0x000000FF);
		}
		pixelsgray = (char *) pixelsgray + infogray->stride;
	}
}

/**
 * Reduces the contrast of the image
 */
void reduceBrightnessOfImage(AndroidBitmapInfo * infogray, void* pixelsgray,
		float brightnessValue) {

	// For reference :  http://www.easyrgb.com/index.php?X=MATH
	//					(0.2126*R) + (0.7152*G) + (0.0722*B) -> Luminance

	int red, green,blue;
	int y,x;

	for (y=0;y<infogray->height;y++) {
		// commenting the line above since 1/4 th of the image gets converted
		// and not the whole, ARGB = 32 bits color system, while
		// uint8_t uses 8bits.(convers 8 bits)
		// uint8_t * line = (uint8_t *)pixelsgray;
		
		uint32_t * line = (uint32_t *)pixelsgray;
		
		for (x=0;x<infogray->width;x++) {

			red = (int)((line[x] & 0x00FF0000) >> 16);
			green = (int)((line[x] & 0x0000FF00) >> 8);
			blue = (int)((line[x] & 0x000000FF));

			red = truncate((int)(red / brightnessValue));
			green = truncate((int)(green / brightnessValue));
			blue= truncate((int)(blue / brightnessValue));

			line[x] = ((red << 16) & 0x00FF0000) |
					((green << 8) & 0x0000FF00) |
					(blue & 0x000000FF);
		}
		pixelsgray = (char *) pixelsgray + infogray->stride;
	}
}

/**
 * Operations for inverting the current image
 */
void invertImage(AndroidBitmapInfo * infogray, void * pixelsgray) {
	int x,y ;
	int red, green, blue;

	for (y=0;y<infogray->height;y++) {
		// commenting the line above since 1/4 th of the image gets converted
		// and not the whole, ARGB = 32 bits color system, while
		// uint8_t uses 8bits.(convers 8 bits)
		// uint8_t * line = (uint8_t *)pixelsgray;
		
		uint32_t * line = (uint32_t *)pixelsgray;
		
		for (x=0;x<infogray->width;x++) {

			// subtract the value from 255
			red = 255 - ((int)((line[x] & 0x00FF0000) >> 16));
			green = 255 - ((int)((line[x] & 0x0000FF00) >> 8));
			blue = 255 - ((int)((line[x] & 0x000000FF)));

			line[x] = ((red << 16) & 0x00FF0000) |
					((green << 8) & 0x0000FF00) |
					(blue & 0x000000FF);
		}
		pixelsgray = (char *) pixelsgray + infogray->stride;
	}
}

/**
 * Applies the blue filter on the image
 */
void convertingImageToBlue(AndroidBitmapInfo * infogray, void * pixelsgray,
		void * pixelsBlue, AndroidBitmapInfo * infoBlue) {

	int x,y;

	for (y=0;y<infogray->height;y++) {
		argb * line = (argb *)pixelsgray;
		argb* lineBlue = (argb *)pixelsBlue;

		for (x=0;x<infogray->width;x++) {
			lineBlue[x].red = 0;
			lineBlue[x].green = line[x].red * 0.704 + 0 * line[x].green - line[x].blue * 0.006;
			lineBlue[x].blue = -1.0 * line[x].blue;//line[x].red * 0.002 + 0.008 * line[x].green + line[x].blue * 0.050;
		}
		pixelsgray = (char *) pixelsgray + infogray->stride;
		pixelsBlue = (char *) pixelsBlue + infoBlue->stride;
	}
}

/**
 * Applies the green filter on the image
 */
void convertingImageToGreen(AndroidBitmapInfo * infogray, void * pixelsgray,
		void * pixelsGreen, AndroidBitmapInfo * infoGreen) {
	int x,y;

	for (y=0;y<infogray->height;y++) {
		argb * line = (argb *)pixelsgray;
		argb* lineGreen = (argb *)pixelsGreen;

		// Turquoise
		//lineGreen[x].red = line[x].red * 0.150 +0.794 * line[x].green + line[x].blue * 0;
		//lineGreen[x].green = line[x].red * 0 + 0.894 * line[x].green + line[x].blue * 0.006;
		//lineGreen[x].blue = line[x].red * 0.002 + 0.008 * line[x].green + line[x].blue * 0.050;

		for (x=0;x<infogray->width;x++) {
			lineGreen[x].red = line[x].red * 0.960 - 0.050 * line[x].green + line[x].blue * 0;
			lineGreen[x].green = -1.0 * line[x].green;
			lineGreen[x].blue = 0.0;
		}
		pixelsgray = (char *) pixelsgray + infogray->stride;
		pixelsGreen = (char *) pixelsGreen + infoGreen->stride;
	}
}

/**
 * Function that converts the image to grayscale image
 */
JNIEXPORT void JNICALL Java_com_example_imageprocessingusingndk_MainActivity_convertToGray(JNIEnv * env,
		jobject jobj, jobject bitmapIn, jobject bitmapOut, jclass cls) {

	// the variables needed declared here
	AndroidBitmapInfo 	infoColor;
	void*				pixelsColor;
	AndroidBitmapInfo	grayImageInfo;
	void*				pixelsGrayColor;
	int					ret;

	if ((ret = AndroidBitmap_getInfo(env, bitmapIn, &infoColor)) < 0) {
		LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
		env->ThrowNew(cls,"AndroidBitmap_getInfo() bitmapIn failed !");
		return;
	}

	if ((ret = AndroidBitmap_getInfo(env, bitmapOut, &grayImageInfo)) < 0) {
		LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
		env->ThrowNew(cls,"AndroidBitmap_getInfo() bitmapOut failed !");
		return;
	}

	if (infoColor.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
		LOG_E("Bitmap format is not RGBA_8888 !");
		env->ThrowNew(cls,"Bitmap format is not RGBA_8888 !");
		return;
	}


	if (grayImageInfo.format != ANDROID_BITMAP_FORMAT_A_8) {
		LOG_E("Bitmap format is not A_8 !");
		env->ThrowNew(cls,"Bitmap format is not A_8 !");
		return;
	}

	//COFFEE_TRY() {
	if ((ret = AndroidBitmap_lockPixels(env, bitmapIn, &pixelsColor)) < 0) {
		LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmapOut, &pixelsGrayColor)) < 0) {
		LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	convertingToGray(pixelsColor, pixelsGrayColor, &infoColor, &grayImageInfo);

	AndroidBitmap_unlockPixels(env,bitmapIn);
	AndroidBitmap_unlockPixels(env, bitmapOut);
}

JNIEXPORT void JNICALL Java_com_example_imageprocessingusingndk_MainActivity_convertToRed(JNIEnv * env,
		jobject jobj, jobject bitmapIn, jobject bitmapOut, jclass cls) {

	// the variables needed declared here
	AndroidBitmapInfo 	infoColor;
	void*				pixelsColor;
	AndroidBitmapInfo	redImageInfo;
	void*				pixelsRedColor;
	int					ret;

	if ((ret = AndroidBitmap_getInfo(env, bitmapIn, &infoColor)) < 0) {
		LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
		env->ThrowNew(cls,"AndroidBitmap_getInfo() bitmapIn failed !");
		return;
	}

	if ((ret = AndroidBitmap_getInfo(env, bitmapOut, &redImageInfo)) < 0) {
		LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
		env->ThrowNew(cls,"AndroidBitmap_getInfo() bitmapOut failed !");
		return;
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmapIn, &pixelsColor)) < 0) {
		LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmapOut, &pixelsRedColor)) < 0) {
		LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	COFFEE_TRY() {
		convertingImageToRed(&infoColor, pixelsColor, pixelsRedColor, &redImageInfo);
		AndroidBitmap_unlockPixels(env,bitmapIn);
		AndroidBitmap_unlockPixels(env, bitmapOut);
	}COFFEE_CATCH() {
		LOG_E("error : %s", coffeecatch_get_message());
	}COFFEE_END();
}

/**
 * Converts the bitmap to green color
 */
JNIEXPORT void JNICALL Java_com_example_imageprocessingusingndk_MainActivity_warmifyImage(JNIEnv * env,
		jobject jobj, jobject bitmapIn, jobject bitmapOut, jclass cls) {

	AndroidBitmapInfo 	infoColor;
	void*				pixelsColor;
	AndroidBitmapInfo	warmImageColor;
	void*				pixelsWarmColor;
	int					ret;

	if ((ret = AndroidBitmap_getInfo(env, bitmapIn, &infoColor)) < 0) {
		LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
		env->ThrowNew(cls,"AndroidBitmap_getInfo() bitmapIn failed !");
		return;
	}

	if ((ret = AndroidBitmap_getInfo(env, bitmapOut, &warmImageColor)) < 0) {
		LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
		env->ThrowNew(cls,"AndroidBitmap_getInfo() bitmapOut failed !");
		return;
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmapIn, &pixelsColor)) < 0) {
		LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmapOut, &pixelsWarmColor)) < 0) {
		LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	COFFEE_TRY() {
		warmifyingImage(&infoColor, pixelsColor, pixelsWarmColor, &warmImageColor);
		AndroidBitmap_unlockPixels(env,bitmapIn);
		AndroidBitmap_unlockPixels(env, bitmapOut);
	}COFFEE_CATCH() {
		LOG_E("error : %s", coffeecatch_get_message());
	}COFFEE_END();
}

/**
 * Converts image from rgb mode to sepia
 */
JNIEXPORT void JNICALL Java_com_example_imageprocessingusingndk_MainActivity_convertToSepia(JNIEnv * env,
		jobject jobj, jobject bitmapIn, jobject bitmapOut, jclass cls) {

	AndroidBitmapInfo 	infoColor;
	void*				pixelsColor;
	AndroidBitmapInfo	sepiaImageColor;
	void*				pixelsSepia;
	int					ret;

	if ((ret = AndroidBitmap_getInfo(env, bitmapIn, &infoColor)) < 0) {
		LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
		env->ThrowNew(cls,"AndroidBitmap_getInfo() bitmapIn failed !");
		return;
	}

	if ((ret = AndroidBitmap_getInfo(env, bitmapOut, &sepiaImageColor)) < 0) {
		LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
		env->ThrowNew(cls,"AndroidBitmap_getInfo() bitmapOut failed !");
		return;
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmapIn, &pixelsColor)) < 0) {
		LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmapOut, &pixelsSepia)) < 0) {
		LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	COFFEE_TRY() {
		convertingImageToSepia(&infoColor, pixelsColor, pixelsSepia, &sepiaImageColor);
		AndroidBitmap_unlockPixels(env,bitmapIn);
		AndroidBitmap_unlockPixels(env, bitmapOut);
	}COFFEE_CATCH() {
		LOG_E("error : %s", coffeecatch_get_message());
	}COFFEE_END();
}

/*
 * increases the brightness of the image based on user input
 */
JNIEXPORT void JNICALL Java_com_example_imageprocessingusingndk_MainActivity_increaseBrightness(JNIEnv * env,
		jobject jobj, jobject bitmapIn, jfloat brightnessValue) {

		AndroidBitmapInfo  	infogray;
	    void*              	pixelsgray;
	    int                	ret;
	    uint8_t 			save;

	    if ((ret = AndroidBitmap_getInfo(env, bitmapIn, &infogray)) < 0) {
	    	LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
	    	return;
	    }

	    if ((ret = AndroidBitmap_lockPixels(env, bitmapIn, &pixelsgray)) < 0) {
	    	LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	    }

	    increasingBrightnessOfImage(&infogray, pixelsgray, brightnessValue);
	    AndroidBitmap_unlockPixels(env, bitmapIn);
}

/**
 * reduces the contrast of the image
 */
JNIEXPORT void JNICALL Java_com_example_imageprocessingusingndk_MainActivity_reduceBrightness(JNIEnv * env,
		jobject jobj, jobject bitmapIn, jfloat brightnessValue) {

		AndroidBitmapInfo  	infogray;
	    void*              	pixelsgray;
	    int                	ret;
	    uint8_t 			save;

	    if ((ret = AndroidBitmap_getInfo(env, bitmapIn, &infogray)) < 0) {
	    	LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
	    	return;
	    }

	    if ((ret = AndroidBitmap_lockPixels(env, bitmapIn, &pixelsgray)) < 0) {
	    	LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	    }

	    reduceBrightnessOfImage(&infogray, pixelsgray, brightnessValue);
	    AndroidBitmap_unlockPixels(env, bitmapIn);
}

/**
 * Converts image to blue scale (almost like applying a blue filter)
 */
JNIEXPORT void JNICALL Java_com_example_imageprocessingusingndk_MainActivity_convertToBlue(JNIEnv * env,
		jobject jobj, jobject bitmapIn, jobject bitmapOut) {

		AndroidBitmapInfo  	infogray;
	    void*              	pixelsgray;
	    AndroidBitmapInfo 	infoBlue;
	    void* 				pixelsBlue;
	    int                	ret;
	    int 				y;
	    int             	x;
	    int					red, green, blue;
	    uint8_t 			save;

	    if ((ret = AndroidBitmap_getInfo(env, bitmapIn, &infogray)) < 0) {
	    	LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
	    	return;
	    }

	    if ((ret = AndroidBitmap_getInfo(env, bitmapOut, &infoBlue)) < 0) {
	    	LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
	    	return;
	    }

	    if ((ret = AndroidBitmap_lockPixels(env, bitmapIn, &pixelsgray)) < 0) {
	    	LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	    }

	    if ((ret = AndroidBitmap_lockPixels(env, bitmapOut, &pixelsBlue)) < 0) {
	    	LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	    }

	    convertingImageToBlue(&infogray, pixelsgray, pixelsBlue, &infoBlue);
	    AndroidBitmap_unlockPixels(env, bitmapIn);
	    AndroidBitmap_unlockPixels(env, bitmapOut);
}

/**
 * Converts image to green (applying a green filter)
 */
JNIEXPORT void JNICALL Java_com_example_imageprocessingusingndk_MainActivity_convertToGreen(JNIEnv * env,
		jobject jobj, jobject bitmapIn, jobject bitmapOut) {

		AndroidBitmapInfo  	infogray;
	    void*              	pixelsgray;
	    AndroidBitmapInfo 	infoGreen;
	    void* 				pixelsGreen;
	    int                	ret;
	    int 				y;
	    int             	x;
	    int					red, green, blue;
	    uint8_t 			save;

	    if ((ret = AndroidBitmap_getInfo(env, bitmapIn, &infogray)) < 0) {
	    	LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
	    	return;
	    }

	    if ((ret = AndroidBitmap_getInfo(env, bitmapOut, &infoGreen)) < 0) {
	    	LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
	    	return;
	    }

	    if ((ret = AndroidBitmap_lockPixels(env, bitmapIn, &pixelsgray)) < 0) {
	    	LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	    }

	    if ((ret = AndroidBitmap_lockPixels(env, bitmapOut, &pixelsGreen)) < 0) {
	    	LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	    }

	    convertingImageToGreen(&infogray, pixelsgray, pixelsGreen, &infoGreen);
	    AndroidBitmap_unlockPixels(env, bitmapIn);
	    AndroidBitmap_unlockPixels(env, bitmapOut);
}

/**
 * Inverts the current image
 */
JNIEXPORT void JNICALL Java_com_example_imageprocessingusingndk_MainActivity_invertImage(JNIEnv * env, jobject jobj, jobject bitmapIn) {
	AndroidBitmapInfo  	infogray;
	void*              	pixelsgray;
	int                	ret;
	int 				y;
	int             	x;
	int					red, green, blue;
	uint8_t 			save;

	if ((ret = AndroidBitmap_getInfo(env, bitmapIn, &infogray)) < 0) {
		LOG_E("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return;
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmapIn, &pixelsgray)) < 0) {
		LOG_E("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	invertImage(&infogray, pixelsgray);
	AndroidBitmap_unlockPixels(env, bitmapIn);
}

//#ifdef __cplusplus
}
//#endif
