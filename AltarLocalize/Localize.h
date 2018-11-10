﻿/*********************************************************************
Copyright (c) 2018
Audi AMapPointonomous Driving Cup. All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3.  All advertising materials mentioning features or use of this software must display the following acknowledgement: ?This product includes software developed by the Audi AG and its contributors for Audi Autonomous Driving Cup.?
4.  Neither the name of Audi nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY AUDI AG AND CONTRIBUTORS AS IS AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL AUDI AG OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

**********************************************************************/

#pragma once

#define CID_CMARKERPOS_DATA_TRIGGERED_FILTER "AltarLocalize.filter.user.aadc.cid"
using namespace adtf_util;
using namespace ddl;
using namespace adtf::ucom;
using namespace adtf::base;
using namespace adtf::streaming;
using namespace adtf::mediadescription;
using namespace adtf::filter;
using namespace std;
using namespace cv;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define RAD2DEG 180/M_PI
#define DEG2RAD M_PI/180

#include "../AltarUtils/MapStructs.h"
#include "SignDetector.h"

/*! the main class of the marker positioning. */
class cLocalize : public cTriggerFunction
{

public:
    std::vector<DebugPoint> debugPointList;
    std::vector<DebugPoint> roadSignList;

    QuadTree* roadLineManager;
    void InsertParticles(double x1,double y1,double x2,double y2,uint id,RoadMarking mark, bool skip = false);


    /*! Default constructor. */
    cLocalize();

    /*! Destructor. */
    virtual ~cLocalize() = default;

    /**
    * Overwrites the Configure
    * This is to Read Properties prepare your Trigger Function
    */
    tResult Configure() override;
    /**
    * Overwrites the Process
    * You need to implement the Reading and Writing of Samples within this function
    * MIND: Do Reading until the Readers queues are empty or use the IPinReader::GetLastSample()
    * This FUnction will be called if the Run() of the TriggerFunction was called.
    */
    tResult Process(tTimeStamp tmTimeOfTrigger) override;

private:

    std::vector<Particle> particles_debug;
//    Particle particleOuterA_1;
//    Particle particleMiddle_1;
//    Particle particleOuterB_1;
//
//    Particle particleOuterA_2;
//    Particle particleMiddle_2;
//    Particle particleOuterB_2;




    int boundary_check;
    int particle_boundary_check;


    double particle_dist_threshold;
    double lookaround_dist_threshold;

        RoadLine roadLines[3];
    std::vector<RoadLine> lines_3;
    std::vector<RoadLine> lines_2;
    std::vector<RoadLine> lines_1;
    int q_count;
    double q_scale_factor;

    /*! configuration file for markers  */
    adtf::base::property_variable<cFilename> m_roadSignFile = cFilename("roadSigns.xml");


    /*! speed scale */
    adtf::base::property_variable<tFloat32> m_f32SpeedScale = 1.0f;
    //adtf::base::property_variable<tFloat32> m_f32SpeedScale = 1000.0f;
    adtf::base::property_variable<tFloat32> m_f32WorldScale = 1000.0f;


    /*! camera offset */
    adtf::base::property_variable<tFloat32> m_f32CameraOffsetLat = 0.0f;
    /*! distance to the rear axle */
    adtf::base::property_variable<tFloat32> m_f32CameraOffsetLon = 0.295f;


    //intialize with signs or nots
    adtf::base::property_variable<tBool> sign_init = tFalse;



    /*! Reader of an InPin roadSign. */
    cPinReader m_oReaderRoadSign;
    /*! Reader of an InPin speed. */
    cPinReader m_oReaderSpeed;
    /*! Reader of an InPin IMU. */
    cPinReader m_oReaderIMU;

    /*! Writer to an OutPin. */
    cPinWriter m_oWriter;

    /*! The codec factory */
    cSampleCodecFactory m_oCodecFactory;

    // MY INFO

    struct ddlParticleDataId
    {
        tSize particleArray;

    } m_ddlParticleDataId;

    /*! The ls structure sample factory */
    adtf::mediadescription::cSampleCodecFactory m_ParticleStructSampleFactory;

    cPinWriter ParticleDataOut;



    struct ddlDebugPointId
    {
        tSize numPoints;
        tSize debugPoints;

    } m_ddlDebugPointId;

    /*! The ls structure sample factory */
    adtf::mediadescription::cSampleCodecFactory m_DebugPointStructSampleFactory;

    cPinWriter DebugPointOut;



    cPinReader DistanceDelta;

    struct ddlPoseCovDataId
            {
                    tSize covArray;

            } m_ddlPoseCovDataId;

    /*! The ls structure sample factory */
    adtf::mediadescription::cSampleCodecFactory m_PoseCovStructSampleFactory;

    /*! Writer to an OutPin. */
    cPinWriter PoseCovDataOut;

    struct ddlLineDataId
    {
        tSize linesFound;
        tSize lineArray;

    } m_ddlLineDataId;

    /*! The ls structure sample factory */
    adtf::mediadescription::cSampleCodecFactory m_LineStructSampleFactory;

    /*! Writer to an OutPin. */
    cPinReader LineDataIn;

    /*! The ddl indices for a tInerMeasUnitData */
    struct
    {
        tSize timeStamp;
        tSize A_x;
        tSize A_y;
        tSize A_z;
        tSize G_x;
        tSize G_y;
        tSize G_z;
        tSize M_x;
        tSize M_y;
        tSize M_z;
        tSize roll;
        tSize pitch;
        tSize yaw;
    } m_ddlInerMeasUnitDataIndex;

    /*! The imu data sample factory */
    adtf::mediadescription::cSampleCodecFactory m_IMUDataSampleFactory;

    /*! The ddl indices for a tSignalValue */
    struct
    {
        tSize timeStamp;
        tSize value;
    } m_ddlSignalDataIndex;

    /*! The signal data sample factory */
    adtf::mediadescription::cSampleCodecFactory m_SignalDataSampleFactory;

    /*! The ddl indices for a tRoadSignExt */
    struct
    {
        tSize id;
        tSize size;
        tSize tvec;
        tSize rvec;
    } m_ddlRoadSignIndex;

    /*! The road sign sample factory */
    adtf::mediadescription::cSampleCodecFactory m_RoadSignSampleFactory;

    /*! The ddl indices for a tPosition */
    struct
    {
        tSize x;
        tSize y;
        tSize radius;
        tSize speed;
        tSize heading;
    } m_ddlPositionIndex;

    /*! The position sample factory */
    adtf::mediadescription::cSampleCodecFactory m_PositionSampleFactory;

    /*! speed estimate */
    tFloat32 m_f32Speed;
    /*! The 32 yaw rate */
    tFloat32 m_f32YawRate;

    /*! The 32 arduino timestamp */
    tUInt32 m_ui32ArduinoTimestamp;

    /*! currently processed road-sign */
    tInt16 m_i16ID;
    /*! Size of the 32 marker */
    tFloat32 m_f32MarkerSize;

    /*! translation vector */
    Mat m_Tvec;
    /*! rotation vector */
    Mat m_Rvec;

    /*! The ticks */
    tTimeStamp m_ticks;

    /*! EKF variables */
    Mat m_state; /*! filter state {X} */
    Mat m_errorCov; /*! error covariance matrix {P} */
    Mat m_processCov; /*! process covariance matrix {Q} */
    Mat m_transitionMatrix; /*! state transition matrix {F} */

    tBool m_isInitialized; /*! initialization state of the filter */



    // Read Lazer Scan Data

    struct ddlLaserScannerDataId
    {
        tSize size;
        tSize scanArray;
    } m_ddlLSDataId;

    adtf::mediadescription::cSampleCodecFactory m_LSStructSampleFactory;
    cPinReader laserDataIn;


    Mat mState;
    Mat mCov;
    Mat mSigmaRoot;
    Mat mMeanWeights;
    Mat mCovWeights;
    Mat mSigmaPoints;
    Mat mCamMeasurementVar;
    Mat mMeasurementPoints;
    Mat mMeasurementMean;
    Mat mMeasurementCov;

    float alpha;
    float beta;
    float k;
    int stateDim;
    int camDim;
    int lidarDim;

    double lidarOffsetLong;
    double lidarOffsetLat;

    // UKF Functions
    tResult GetSigmaPoints(Mat &state, Mat& covariance, Mat &sigmaPoint, double alpha, double beta, double k);
    tResult GetWeights(Mat &state, Mat& weightsMean, Mat & weightsCov, double alpha, double beta, double k);
    tResult MotionModel(Mat &sigmaPoints, double dt, double yawRate, double velocity);
    tResult CameraModel(Mat &sigmaPoints, Mat &predPoints, RoadSignPose sign);
    tResult KalmanUpdate(Mat &measurmentData, Mat &measureVector, Mat &measureCov, Mat &crossVariance, Mat &stateVector, Mat &stateCov, std::vector<bool> angles);
    tResult LidarModel(Mat &sigmaPoints, Mat &predPoints, Pose sign);


    // Update Functions

    tResult LaserUpdate(tTimeStamp tmTimeOfTrigger, const adtf::streaming::ISample &oSample);


    // UKF Variables

    SignDetector* RoadSignDetector;


    tResult UTMotion(Mat &sigmaPoints, Mat& weightsMean, Mat &weightsCov, Mat &meanVector, Mat &covariance, std::vector<bool> angles);
    tResult UTSensor(Mat &measurePoints, Mat &sigmaPoints, Mat& weightsMean, Mat &weightsCov, Mat &stateVector, Mat &measureVector, std::vector<bool> angles, std::vector<bool> stateAngles, Mat &covariance, Mat &crossVariance);


        //Vizulation Functions
    tResult SendDebugPoints();



    // Transform Functions
    MapPoint LaserToGlobal(tPolarCoordiante laserPoint);






        /*! storage for the roadsign data */
    vector<roadSign> m_roadSigns;

    /*! Number of 32s */
    tInt m_ui32Cnt;

    /*!
     * helper functions.
     *
     * \param   x   A tFloat32 to process.
     * \param   y   A tFloat32 to process.
     *
     * \return  A tFloat32.
     */
    tFloat32 mod(tFloat32 x, tFloat32 y);

    /*!
     * Normalize angle.
     *
     * \param   alpha   The alpha.
     * \param   center  The center.
     *
     * \return  A tFloat32.
     */
    tFloat32 normalizeAngle(tFloat32 alpha, tFloat32 center);

    /*!
     * Angle difference.
     *
     * \param   angle1  The first angle.
     * \param   angle2  The second angle.
     *
     * \return  A tFloat32.
     */
    tFloat32 angleDiff(tFloat32 angle1, tFloat32 angle2);

    /*!
     * Gets the time.
     *
     * \return  The time.
     */
    tTimeStamp GetTime();

    /*!
     * Process the iner meas unit sample.
     *
     * \param   tmTimeOfTrigger The time time of trigger.
     * \param   oSample         The sample.
     *
     * \return  Standard Result Code.
     */
    tResult ProcessInerMeasUnitSample(tTimeStamp tmTimeOfTrigger, const adtf::streaming::ISample &oSample);

    /*!
     * Process the road sign structure extent.
     *
     * \param   tmTimeOfTrigger The time time of trigger.
     * \param   oSample         The sample.
     *
     * \return  Standard Result Code.
     */
    tResult ProcessRoadSignStructExt(tTimeStamp tmTimeOfTrigger, const adtf::streaming::ISample &oSample);

    /*!
     * Sends a position structure.
     *
     * \param   timeOfFix   The time of fix.
     * \param   f32X        The 32 x coordinate.
     * \param   f32Y        The 32 y coordinate.
     * \param   f32Radius   The 32 radius.
     * \param   f32Heading  The 32 heading.
     * \param   f32Speed    The 32 speed.
     *
     * \return  Standard Result Code.
     */
    tResult sendPositionStruct(const tTimeStamp &timeOfFix, const tFloat32 &f32X, const tFloat32 &f32Y, const tFloat32 &f32Radius,
                               const tFloat32 &f32Heading, const tFloat32 &f32Speed);

};
