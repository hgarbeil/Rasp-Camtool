#include "Cam.h"
#include <unistd.h>
#include "jpeglib.h"
using namespace std ;

CamC::CamC () {
	cam = new RaspiCam () ;
	w= 320 ;
	h= 640 ;

}

CamC::~CamC() {
	//cam->close() ;
}	

bool CamC::camopen () {

	// try to open the camera
	int status = cam->open() ;
	// print out status
	cout << "Camera open : "<< status << endl ;
	// wait for camera to stabilize
	usleep(1000000) ;
	return -1 ;
}

void CamC::camparams() {
	w = cam->getWidth() ;
	h = cam->getHeight() ;
	cout << "Image sizes are : " << w << " " << h << endl ;
	


}

void CamC::getImage (unsigned char *data) {
	cam->grab() ;
	cam->retrieve (data, raspicam::RASPICAM_FORMAT_RGB) ;
	FILE *fin = fopen ("Test.dat", "w") ;
	fwrite ((char *) data, 1, w * h * 3, fin) ;
	fclose (fin) ;
}


void CamC::writeJPeg (const char *fname, unsigned char *data) {

	FILE *outfile ;
	struct jpeg_compress_struct cinfo ;
	struct jpeg_error_mgr jerr ;
	
	JSAMPROW row_pointer[1] ;
	int row_stride, quality ;

	quality = 75 ;
	cinfo.err = jpeg_std_error (&jerr) ;
	jpeg_create_compress (&cinfo) ;
	
	if ((outfile = fopen (fname, "wb")) == NULL) {
		fprintf(stderr, "can't open %s\r\n", fname) ;
		return ;
	}
	jpeg_stdio_dest(&cinfo, outfile) ;


	/* image width and height, in pixels */
	cinfo.image_width = w ;
  	cinfo.image_height = h ;
	/* # of color components per pixel */
  	cinfo.input_components = 3;		
	/* colorspace of input image *//* colorspace of input image */
	cinfo.in_color_space = JCS_RGB; 	

	jpeg_set_defaults(&cinfo) ;
	jpeg_set_quality(&cinfo, quality, TRUE) ;
	jpeg_start_compress (&cinfo,  TRUE) ;
	row_stride = w * 3 ;

	while (cinfo.next_scanline < cinfo.image_height) {
		row_pointer[0] = &data[cinfo.next_scanline * row_stride] ;
		jpeg_write_scanlines(&cinfo, row_pointer, 1) ;
	}
	jpeg_finish_compress(&cinfo) ;
	fclose (outfile) ;
	jpeg_destroy_compress (&cinfo) ;
}

	
	

int main (int argc, char *argv[] ) {
	int w, h ;
	unsigned char *data ;
	CamC c ;
	c.camopen () ;
	c.camparams() ;
	w = c.w ;
	h = c.h ;
	data = new unsigned char [w * h * 3] ;
	c.getImage (data) ;
	c.writeJPeg ("test.jpg", data) ;
	
	delete [] data ;
	
}
	



