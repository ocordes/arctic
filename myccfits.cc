//TO COMPILE THIS PROGRAM

//g++ myccfits.cc -I/afs/slac/g/ki/include -I/afs/slac/g/ki/include/CCfits -L/afs/slac/g/ki/lib -lCCfits

// The CCfits headers are expected to be installed in a subdirectory of
// the include path.

// The <CCfits> header file contains all that is necessary to use both the CCfits
// library and the cfitsio library (for example, it includes fitsio.h) thus making
// all of cfitsio's macro definitions available.

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// this includes 12 of the CCfits headers and will support all CCfits operations.
// the installed location of the library headers is $(ROOT)/include/CCfits

// to use the library either add -I$(ROOT)/include/CCfits or #include <CCfits/CCfits>
// in the compilation target.

#include <CCfits/CCfits>
#include <Image.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <fitsio.h>
    // The library is enclosed in a namespace.
        
    using namespace CCfits;




int main();
int writeImage();
int writeAscii();
int writeBinary();
int copyHDU();
int selectRows();
int readHeader(); 
int readImage();
int readTable();

int main()
{


     FITS::setVerboseMode(true);

     try
                     
     {

        if (!writeImage()) std::cerr << " writeImage() \n";
/*        if (!writeAscii()) std::cerr << " writeAscii() \n";
        if (!writeBinary()) std::cerr << " writeBinary()  \n";
        if (!copyHDU()) std::cerr << " copyHDU() \n";
        if (!readHeader()) std::cerr << " readHeader() \n";
        if (!readImage()) std::cerr << " readImage() \n";
        if (!readTable()) std::cerr << " readTable() \n";
        if (!selectRows()) std::cerr << " selectRows() \n";
*/
     }
     catch (FitsException&) 
     // will catch all exceptions thrown by CCfits, including errors
     // found by cfitsio (status != 0)
     {
             
        std::cerr << " Fits Exception Thrown by test function \n";       
             
     }
    return 0;
}

int writeImage()
{

    // Create a FITS primary array containing a 2-D image               
    // declare axis arrays.    
    long naxis    =   2;      
    long naxes[2] = { 4096, 4096 };   
    
    // declare auto-pointer to FITS at function scope. Ensures no resources
    // leaked if something fails in dynamic allocation.
    std::auto_ptr<FITS> pFits(0);
      
    try
    {                
        // overwrite existing file if the file already exists.
            
        const std::string fileName("!/nfs/slac/g/ki/ki08/lsst/lsstgeant4/atestfil.fit");            
        
        // Create a new FITS object, specifying the data type and axes for the primary
        // image. Simultaneously create the corresponding file.
        
        // this image is unsigned short data, demonstrating the cfitsio extension
        // to the FITS standard.
        
        pFits.reset( new FITS(fileName , USHORT_IMG , naxis , naxes ) );
    }
    catch (FITS::CantCreate)
    {
          // ... or not, as the case may be.
          return -1;       
    }
    
    // references for clarity.
    
    long& vectorLength = naxes[0];
    long& numberOfRows = naxes[1];
    long nelements(1); 
    
    
    // Find the total size of the array. 
    // this is a little fancier than necessary ( It's only
    // calculating naxes[0]*naxes[1]) but it demonstrates  use of the 
    // C++ standard library accumulate algorithm.
    
    nelements = std::accumulate(&naxes[0],&naxes[naxis],1,std::multiplies<long>());
           
    // create a new image extension with a 300x300 array containing float data.
    
    std::vector<long> extAx(2,4096);
    string newName ("NEW-EXTENSION");
    //ExtHDU* imageExt = pFits->addImage(newName,FLOAT_IMG,extAx);
    
    // create a dummy row with a ramp. Create an array and copy the row to 
    // row-sized slices. [also demonstrates the use of valarray slices].   
    // also demonstrate implicit type conversion when writing to the image:
    // input array will be of type float.
    
    std::valarray<int> row(vectorLength);
    for (long j = 0; j < vectorLength; ++j) row[j] = j;
    std::valarray<int> array(nelements);
    for (int i = 0; i < numberOfRows; ++i)
    {
//      array[std::slice(vectorLength*static_cast<int>(i),vectorLength,1)] = row + i;     
    }
    
    // create some data for the image extension.
    long extElements = std::accumulate(extAx.begin(),extAx.end(),1,std::multiplies<long>()); 
    /*std::valarray<float> ranData(extElements);
    const float PIBY (M_PI/150.);
    for ( int jj = 0 ; jj < extElements ; ++jj) 
    {
            float arg = PIBY*jj;
            ranData[jj] = std::cos(arg);
    }
 */
    long  fpixel(1);
    
    // write the image extension data: also demonstrates switching between
    // HDUs.
    // imageExt->write(fpixel,extElements,ranData);
    
    //add two keys to the primary header, one long, one complex.
    
    long exposure(1500);
    std::complex<float> omega(std::cos(2*M_PI/3.),std::sin(2*M_PI/3));
    pFits->pHDU().addKey("EXPOSURE", exposure,"Total Exposure Time"); 
    pFits->pHDU().addKey("OMEGA",omega," Complex cube root of 1 ");  

    
    // The function PHDU& FITS::pHDU() returns a reference to the object representing 
    // the primary HDU; PHDU::write( <args> ) is then used to write the data.
    
    pFits->pHDU().write(fpixel,nelements,array);
    
    
    // PHDU's friend ostream operator. Doesn't print the entire array, just the
    // required & user keywords, and is provided largely for testing purposes [see 
    // readImage() for an example of how to output the image array to a stream].
    
    std::cout << pFits->pHDU() << std::endl;

    return 0;
}


