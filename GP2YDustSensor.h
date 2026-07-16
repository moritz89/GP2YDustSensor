#include <stdint.h>

#include <memory>

enum GP2YDustSensorType
{
    GP2Y1010AU0F,
    GP2Y1014AU0F
};

class GP2YDustSensor
{
    private:
        static const uint32_t SAMPLE_PERIOD_US = 10000;
        GP2YDustSensorType type;
        uint32_t maxAdc;
        uint8_t ledOutputPin;
        uint8_t analogReadPin;
        float zeroDustVoltage;
        float minDustVoltage;
        float minZeroDustVoltage;
        float maxZeroDustVoltage;
        float typZeroDustVoltage;
        float currentBaselineCandidate;
        bool hasBaselineCandidate;
        uint16_t readCount = 0;
        float calibrationFactor;
        float sensitivity;
        std::unique_ptr<int16_t[]> runningAverageBuffer;
        int runningAverageCount;
        int nextRunningAverageCounter;
        int runningAverageCounter;
        const uint8_t BASELINE_CANDIDATE_MIN_READINGS = 10;
        bool measurementInProgress;
        bool measurementReady;
        uint16_t measurementTargetSamples;
        uint16_t measurementCollectedSamples;
        uint32_t measurementRawTotal;
        uint32_t nextSampleAtUs;
        uint16_t lastDustDensity;

        uint16_t calculateDustDensityFromRaw(uint16_t avgRaw);

    protected:
        uint16_t readDustRawOnce();
        void updateRunningAverage(uint16_t dustDensity);

    public:
        GP2YDustSensor(GP2YDustSensorType type,
                       uint8_t ledOutputPin,
                       uint8_t analogReadPin,
                       uint16_t runningAverageCount = 60);
        ~GP2YDustSensor() = default;
        void begin();
        uint16_t getDustDensity(uint16_t numSamples = 20);
        bool startDustDensityMeasurement(uint16_t numSamples = 20);
        bool handleDustDensityMeasurement();
        bool isDustDensityMeasurementInProgress() const;
        bool isDustDensityMeasurementReady() const;
        uint16_t getLastDustDensity() const;
        uint32_t getDustDensityRemainingTimeUs() const;
        uint16_t getRunningAverage();
        float getBaseline();
        void setBaseline(float zeroDustVoltage);
        float getBaselineCandidate();
        void setSensitivity(float sensitivity);
        float getSensitivity();
        void setCalibrationFactor(float slope);
};
