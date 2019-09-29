///////////////////////////////////////////////////////////////////////////////////////
//  QuatSpec.cpp
//
//  Quaternion Class Methods Specialization
//
///////////////////////////////////////////////////////////////////////////////////////
#include "PlatformTypes.h"

#include <math.h>
#include <memory.h>

#include "MathConst.h"
#include "MathDefs.h"


#include "MathPrim.h"
#include "Trigonometry.h"

#include "MathLibDefs.h"

#include "Vector.h"
#include "Matrix.h"
#include "Quaternion.h"


///////////////////////////////////////////////////////////////////////////////////////
template class CMQuaternion<float>;
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// Quaternion Compression
// Quaternion Must Be Normalized
// This Is Method Of The Three Small Values
// We Cut Out The Largest Element Of A Quaternion And Compress Three Left Elements
// Each Element We Convert From [-1/sqrt2, 1/sqrt2] To [0,1023] And Write In
// 10 Bits Of Integer Value
//  Largest Value Index    Value     Value     Value
// |      2 Bits        | 10 Bits | 10 Bits | 10 Bits |

int32t CMQuatCompress(CMQuaternion<float> &Q)
{
    int32t index = 0;
    float *qi = &Q.q[0];
    float maxval = qi[0];
    
    //Searching Index Of The Largest Value In This Quaternion
    for (int32t i = 1; i < 4; i++)
    {
        if (qi[i] > qi[i-1])
        {
            index = i;
            maxval = qi[i];
        }
    }

    CMQuaternion<float> quat;
    if (mless0(maxval))
        quat = -Q;
    else
        quat = Q;

    //Starting Quaternion Compression Into 32 bit Integer Value
    int32t ciquat = 0;

    //Compressing Index Of The Largest Part Of Quaternion
    ciquat = index<<30;

    //Calc Smallest Value Used In Interpolation 
    float quant = M_SQRT_2/1023.0f;
    
    qi = &quat.q[0];
    int32t offset = 2;
    for(int32t i = 0; i < 4; i++)
    {
        if (i != index)
        {
            //Packing Values From [-1/sqrt2, 1/sqrt2] To [0,sqrt2]
            float val = qi[i] + M_SQRT_1_2;
            
            //Converting To Range [0,1023]
            val /= quant;
            
            //Rounding Value And Converting To Integer Number
            int32t ival = mftoi(val);
            ival = mclamp(0,1023,ival);
          
            //Writing Converted Value In Appropriate 10 bits Of Integer Value
            ciquat = ciquat | (ival<<10*offset);
            offset = offset - 1;
        }
    }


    return ciquat;
}

///////////////////////////////////////////////////////////////////////////////////////
// Quaternion Decompression
void CMQuatDecmpress(CMQuaternion<float> &Q, int32t ciquat)
{
    //Get Index Of The Largest Element Of Compressed Quaternion
    int32t index = mabs(ciquat >> 30);

    //Calc Smallest Value Used In Interpolation 
    float quant = M_SQRT_2/1023.0f;

    int32t offset = 2;
    float *qi = &Q.q[0];
    qi[index] = 0.0f;

    for(int32t i = 0; i < 4; i++)
    {
        if (i != index)
        {
            //Getting Element Of A Quaternion. Bit Offset And Eliminating Rest Values.
            int32t ival = (ciquat>>10*offset) & 0x3ff; //0x3ff - dec: 1023 or bin: 1111111111 
            float fval = mitof(ival);
            fval *= quant;
            //Moving To Normal Value
            fval -= M_SQRT_1_2;
            qi[i] = fval;

            offset = offset - 1;
        }
    }
    
    qi[index] = MQUATSQRT(1.0f - (qi[0]*qi[0] + qi[1]*qi[1] + qi[2]*qi[2] + qi[3]*qi[3]));
}

////////////////////////////////////////////////////////////////////////////
// Quaternion Compression Polar Coding Method
// Quaternion Must Be Normalized
//   SignX   SignY   SignZ   Normalizing W   PitchYaw Value
// | 1 bit | 1 bit | 1 bit |    11 bits    |     18 bits    |
int32t CMQuatPolarCoding(CMQuaternion<float> &Q)
{
    CMQuaternion<float> quat;
    if (mless0(Q.w))//If Less 0 Negating Quaternion
        quat = -Q;
    else
        quat = Q;

    float *qi = &quat.q[0];

    int32t signs[] = {0,0,0};

    //Checking Vector(x,y,z) Signes And Reversing Vector Values
    for(int32t i = 0; i < 3; i++)
    {
        if(mless0(qi[i]))
            signs[i] = 0x1;
           
        qi[i] = mabs(qi[i]); 
    }

    int32t bitoffset = 31;
    int32t ciquat = 0;

    //Writing Vector Elements Signes
    for(int32t i = 0; i < 3; i++)
    {
        ciquat = ciquat | (signs[i] << (bitoffset-i));
    }

    // Converting W Element Of The Quaternion Into 11 Bit Integer Value
    float fw = qi[3]*2047.0f;
    int32t iw = mftoi(fw);

    // Writing 11 Bit Value In Compressed Quaternion
    ciquat = ciquat | (iw << 18);

    float lenxz2 = qi[0]*qi[0] + qi[2]*qi[2];
    float sqxz = MQUATSQRT(lenxz2);

    // If Length Of The Vector Projected On XZ Plane Is Equal Zero
    // Than Vector Is Targeting Up Along Y Axis And There Is No Need
    // To Process Calcualtions
    if (!mnear0(sqxz))
    {
        //Pitch (Vertical Angle)
        float ksi = MQUATATAN(qi[1]/sqxz);

        //Inverting Angle For Proper Polar Compression
        ksi = M_PI_2 - ksi;
        //Converting Ksi To Range [0,511]
        float vquant = 511.0f/M_PI_2;
        float iksi = ksi*vquant;

        int32t irow = mftoi(iksi);//Getting Column Row
        irow = mclamp(0,511,irow);

        int32t elem = irow*irow;//First Element

        //Getting Value For Horizontal Angle
        if(!mnear0(qi[2]))
        {
            int32t columnNum = (irow+1)*(irow+1) - elem;
            if (mnear0(qi[0]))
            {
                elem = elem + columnNum - 1;
            }
            else
            {
                float hquant = mitof(columnNum - 1)/M_PI_2;
                float theta = MQUATATAN(qi[2]/qi[0]); //Yaw (Horizontal Angle)
                theta = theta*hquant;
                //Getting Column
                int32t icolumn = mftoi(theta);
                icolumn = mclamp(0,columnNum-1,icolumn);
                elem = elem + icolumn;
            }
        }

        //Saving Coded YawPitch Value As A Single Number
        ciquat = ciquat | elem;
    }
  
    return ciquat;
}

////////////////////////////////////////////////////////////////////////////
// Quaternion Polar Coding Decompression
void CMQuatPolarDecoding(CMQuaternion<float> &Q, int32t ciquat)
{
    // Getting W Element Of The Quaternion
    int32t iw = (ciquat >> 18) & 0x7FF;
    float fw = mitof(iw)/2047.0f;


    //Long Process Of Extracting X,Y,Z Element Of 3D Vector
    
    //Getting Angles Of Rotations
    int32t ielem = ciquat & 0x3FFFF;
    float felem = mitof(ielem);
    float frow = mfloor(MQUATSQRT(felem));
    int32t irow = mftoi(frow);
    int32t irow2 = irow*irow;
    int32t icolumn = ielem - irow2;

    int32t icolumnNum = (irow+1)*(irow+1) - irow2;

    float hangle = mitof(icolumn)*M_PI_2/mitof(icolumnNum - 1);
    float vangle = M_PI_2 - frow*(M_PI_2/511.0f);
    
    float hngl_sin,hngl_cos;
    float vngl_sin,vngl_cos;

    MQUATSINCOS(hangle,hngl_sin,hngl_cos);
    MQUATSINCOS(vangle,vngl_sin,vngl_cos);

    float *qi = (float*)Q;

    qi[0] = vngl_cos*hngl_cos;
    qi[1] = vngl_sin;
    qi[2] = vngl_cos*hngl_sin;
    qi[3] = fw;

    // Getting Normalizing Vector's Values
    float normVal = MQUATSQRT(1.0f + fw*fw);

    // Normalizng Vector Elements And Getting Signs
    int32t bitoffset = 31;
    for(int32t i = 0; i < 3; i++)
    {
        qi[i] = qi[i]/normVal;
        
        if ((ciquat >> (bitoffset-i)) & 0x1)
            qi[i] = -qi[i];
    }
}


////////////////////////////////////////////////////////////////////////////
// Quaternion Compression Polar Coding Method Saving Two Angles
// Quaternion Must Be Normalized
//   SignX   SignY   SignZ   Normalizing W   Vertical Angle   Horizontal Angle
// | 1 bit | 1 bit | 1 bit |    11 bits    |     9 bits     |      9 bits      |
int32t CMQuatPolarAngleCoding(CMQuaternion<float> &Q)
{
    CMQuaternion<float> quat;
    if (mless0(Q.w))//If Less 0 Negating Quaternion
        quat = -Q;
    else
        quat = Q;

    float *qi = &quat.q[0];

    int32t signs[] = {0,0,0};

    //Checking Vector(x,y,z) Signes And Reversing Vector Values
    for(int32t i = 0; i < 3; i++)
    {
        if(mless0(qi[i]))
            signs[i] = 0x1;
           
        qi[i] = mabs(qi[i]); 
    }

    int32t bitoffset = 31;
    int32t ciquat = 0;

    //Writing Vector Elements Signes
    for(int32t i = 0; i < 3; i++)
    {
        ciquat = ciquat | (signs[i] << (bitoffset-i));
    }

    // Converting W Element Of The Quaternion Into 11 Bit Integer Value
    float fw = qi[3]*2047.0f;
    int32t iw = mftoi(fw);

    // Writing 11 Bit Value In Compressed Quaternion
    ciquat = ciquat | (iw << 18);

    float lenxz2 = qi[0]*qi[0] + qi[2]*qi[2];
    float sqxz = MQUATSQRT(lenxz2);

    float vangle = 0.0f;
    float hangle = 0.0f;

    float quant = 511.0f/M_PI_2;

    //Vertical Angle
    vangle = MQUATATAN(qi[1]/sqxz);
    //Inverting Angle For Proper Polar Compression
    vangle = M_PI_2 - vangle;
    
    //Getting Value For Horizontal Angle
    hangle = MQUATATAN(qi[2]/qi[0]);

    // Converting And Saving Vertical Angle
    vangle = vangle*quant;
    int32t ivangle = mftoi(vangle);
    ciquat = ciquat | (ivangle << 9);

    // Converting And Saving Horizontal Angle
    hangle = hangle*quant;
    int32t ihangle = mftoi(hangle);
    ciquat = ciquat | ihangle;
    

    return ciquat;
}

////////////////////////////////////////////////////////////////////////////
// Quaternion Polar Coding Decompression Decoding Two Angles
void CMQuatPolarAngleDecoding(CMQuaternion<float> &Q, int32t ciquat)
{
    // Getting W Element Of The Quaternion
    int32t iw = (ciquat >> 18) & 0x7FF;
    float fw = mitof(iw)/2047.0f;


    ////////////////////////////////////////////////////////////////////////////
    // Getting Angles
    int32t ivangle = (ciquat >> 9) & 0x1FF;
    int32t ihangle = ciquat & 0x1FF;
    float scale = M_PI_2/511.0f;

    float hangle = mitof(ihangle)*scale;
    float vangle = M_PI_2 - mitof(ivangle)*scale;
    
    float hngl_sin,hngl_cos;
    float vngl_sin,vngl_cos;

    MQUATSINCOS(hangle,hngl_sin,hngl_cos);
    MQUATSINCOS(vangle,vngl_sin,vngl_cos);

    float *qi = (float*)Q;

    qi[0] = vngl_cos*hngl_cos;
    qi[1] = vngl_sin;
    qi[2] = vngl_cos*hngl_sin;
    qi[3] = fw;

    // Getting Normalizing Vector's Values
    float normVal = MQUATSQRT(1.0f + fw*fw);

    // Normalizng Vector Elements And Getting Signs
    int32t bitoffset = 31;
    for(int32t i = 0; i < 3; i++)
    {
        qi[i] = qi[i]/normVal;
        
        if ((ciquat >> (bitoffset-i)) & 0x1)
            qi[i] = -qi[i];
    }
}