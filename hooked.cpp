#include <windows.h>
#include <detours/detours.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <Xinput.h>
#include <GameInput.h>

// Let's use randomly generated binary string for device identification
#define EMU_DEVICE_ID { 0x68, 0xC1, 0xE3, 0x1E, 0xF6, 0x5C, 0xB2, 0xC0, 0xFC, 0xBB, 0x5A, 0x26, 0xB4, 0x6A, 0x42, 0x39, 0xCF, 0x83, 0x4E, 0xEB, 0x03, 0xB8, 0xE6, 0x10, 0xAF, 0x34, 0x60, 0x77, 0xF3, 0x13, 0x7D, 0x75 }

//#define VERBOSE
#ifdef VERBOSE
#define LOG_FUNCTION_CALL log(__FUNCSIG__)
#else
#define LOG_FUNCTION_CALL
#endif

std::ofstream logFile;
char buffer[1024];

#ifdef VERBOSE
std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    std::tm localTime{};
    localtime_s(&localTime, &now); // Windows

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}
#endif

void log(std::string message) {
#ifdef VERBOSE
    if (!logFile.is_open()) {
        logFile.open("xinput.log", std::ios::app | std::ios::out);
        sprintf(buffer, "\n\n > New process '%s'", GetCommandLine());
        log(buffer);
    }

    logFile << "[" << getCurrentTimestamp() << "] " << message << std::endl;
#endif
}

#ifdef VERBOSE
std::string GetInfo(const GameInputDeviceInfo* info) {
    std::ostringstream oss;
    oss << "Info Size: " << info->infoSize << "\n";
    oss << "Vendor ID: 0x" << std::hex << info->vendorId << "\n";
    oss << "Product ID: 0x" << std::hex << info->productId << "\n";
    oss << "Revision Number: 0x" << std::hex << info->revisionNumber << "\n";
    oss << "Interface Number: " << std::dec << (int)info->interfaceNumber << "\n";
    oss << "Collection Number: " << (int)info->collectionNumber << "\n";
    oss << "Usage: " << info->usage.id << " " << info->usage.page << "\n";
    oss << "Hardware Version: " << info->hardwareVersion.major << "." << info->hardwareVersion.minor << "\n";
    oss << "Firmware Version: " << info->firmwareVersion.major << "." << info->firmwareVersion.minor << "\n";
    oss << "Device ID: ";
    for (int i = 0; i < 32; i++)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)info->deviceId.value[i];
    oss << "\n";
    oss << "Device Root ID: ";
    for (int i = 0; i < 32; i++)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)info->deviceRootId.value[i];
    oss << "\n";
    oss << "Device Family: " << info->deviceFamily << "\n";
    oss << "Capabilities: " << info->capabilities << "\n";
    oss << "Supported Input: " << std::hex << info->supportedInput << "\n";
    oss << "Supported Rumble Motors: " << std::dec << info->supportedRumbleMotors << "\n";
    oss << "Input Report Count: " << info->inputReportCount << "\n";
    oss << "Output Report Count: " << info->outputReportCount << "\n";
    oss << "Feature Report Count: " << info->featureReportCount << "\n";
    oss << "Controller Axis Count: " << info->controllerAxisCount << "\n";
    oss << "Controller Button Count: " << info->controllerButtonCount << "\n";
    oss << "Controller Switch Count: " << info->controllerSwitchCount << "\n";
    oss << "Touch Point Count: " << info->touchPointCount << "\n";
    oss << "Touch Sensor Count: " << info->touchSensorCount << "\n";
    oss << "Force Feedback Motor Count: " << info->forceFeedbackMotorCount << "\n";
    oss << "Haptic Feedback Motor Count: " << info->hapticFeedbackMotorCount << "\n";
    oss << "Device String Count: " << info->deviceStringCount << "\n";
    oss << "Device Descriptor Size: " << info->deviceDescriptorSize << "\n";
    oss << "Supported System Buttons: " << info->supportedSystemButtons << "\n";
    oss << "Input Report Info: " << info->inputReportInfo << "\n";
    oss << "Output Report Info: " << info->outputReportInfo << "\n";
    oss << "Feature Report Info: " << info->featureReportInfo << "\n";
    oss << "Controller Axis Info: " << info->controllerAxisInfo << "\n";
    oss << "Controller Button Info: " << info->controllerButtonInfo << "\n";
    oss << "Controller Switch Info: " << info->controllerSwitchInfo << "\n";
    oss << "Keyboard Info: " << info->keyboardInfo << "\n";
    oss << "Mouse Info: " << info->mouseInfo << "\n";
    oss << "Touch Sensor Info: " << info->touchSensorInfo << "\n";
    oss << "Motion Info: " << info->motionInfo << "\n";
    oss << "Arcade Stick Info: " << info->arcadeStickInfo << "\n";
    oss << "Flight Stick Info: " << info->flightStickInfo << "\n";
    oss << "Gamepad Info: " << info->gamepadInfo << "\n";
    if (info->gamepadInfo) {
        oss << "  Menu Button Label: " << info->gamepadInfo->menuButtonLabel << "\n";
        oss << "  View Button Label: " << info->gamepadInfo->viewButtonLabel << "\n";
        oss << "  A Button Label: " << info->gamepadInfo->aButtonLabel << "\n";
        oss << "  B Button Label: " << info->gamepadInfo->bButtonLabel << "\n";
        oss << "  X Button Label: " << info->gamepadInfo->xButtonLabel << "\n";
        oss << "  Y Button Label: " << info->gamepadInfo->yButtonLabel << "\n";
        oss << "  Dpad Up Label: " << info->gamepadInfo->dpadUpLabel << "\n";
        oss << "  Dpad Down Label: " << info->gamepadInfo->dpadDownLabel << "\n";
        oss << "  Dpad Left Label: " << info->gamepadInfo->dpadLeftLabel << "\n";
        oss << "  Dpad Right Label: " << info->gamepadInfo->dpadRightLabel << "\n";
        oss << "  Left Shoulder Button Label: " << info->gamepadInfo->leftShoulderButtonLabel << "\n";
        oss << "  Right Shoulder Button Label: " << info->gamepadInfo->rightShoulderButtonLabel << "\n";
        oss << "  Left Thumbstick Button Label: " << info->gamepadInfo->leftThumbstickButtonLabel << "\n";
        oss << "  Right Thumbstick Button Label: " << info->gamepadInfo->rightThumbstickButtonLabel << "\n";
    }
    oss << "Racing Wheel Info: " << info->racingWheelInfo << "\n";
    oss << "UI Navigation Info: " << info->uiNavigationInfo << "\n";
    if (info->uiNavigationInfo) {
        oss << "  Menu Button Label: " << info->uiNavigationInfo->menuButtonLabel << "\n";
        oss << "  View Button Label: " << info->uiNavigationInfo->viewButtonLabel << "\n";
        oss << "  Accept Button Label: " << info->uiNavigationInfo->acceptButtonLabel << "\n";
        oss << "  Cancel Button Label: " << info->uiNavigationInfo->cancelButtonLabel << "\n";
        oss << "  Up Button Label: " << info->uiNavigationInfo->upButtonLabel << "\n";
        oss << "  Down Button Label: " << info->uiNavigationInfo->downButtonLabel << "\n";
        oss << "  Left Button Label: " << info->uiNavigationInfo->leftButtonLabel << "\n";
        oss << "  Right Button Label: " << info->uiNavigationInfo->rightButtonLabel << "\n";
        oss << "  Context Button 1 Label: " << info->uiNavigationInfo->contextButton1Label << "\n";
        oss << "  Context Button 2 Label: " << info->uiNavigationInfo->contextButton2Label << "\n";
        oss << "  Context Button 3 Label: " << info->uiNavigationInfo->contextButton3Label << "\n";
        oss << "  Context Button 4 Label: " << info->uiNavigationInfo->contextButton4Label << "\n";
        oss << "  Page Up Button Label: " << info->uiNavigationInfo->pageUpButtonLabel << "\n";
        oss << "  Page Down Button Label: " << info->uiNavigationInfo->pageDownButtonLabel << "\n";
        oss << "  Page Left Button Label: " << info->uiNavigationInfo->pageLeftButtonLabel << "\n";
        oss << "  Page Right Button Label: " << info->uiNavigationInfo->pageRightButtonLabel << "\n";
        oss << "  Scroll Up Button Label: " << info->uiNavigationInfo->scrollUpButtonLabel << "\n";
        oss << "  Scroll Down Button Label: " << info->uiNavigationInfo->scrollDownButtonLabel << "\n";
        oss << "  Scroll Left Button Label: " << info->uiNavigationInfo->scrollLeftButtonLabel << "\n";
        oss << "  Scroll Right Button Label: " << info->uiNavigationInfo->scrollRightButtonLabel << "\n";
        oss << "  Guide Button Label: " << info->uiNavigationInfo->guideButtonLabel << "\n";
    }
    oss << "Force Feedback Motor Info: " << info->forceFeedbackMotorInfo << "\n";
    oss << "Haptic Feedback Motor Info: " << info->hapticFeedbackMotorInfo << "\n";
    oss << "Display Name: " << info->displayName << "\n";
    oss << "Device Strings: " << info->deviceStrings << "\n";
    oss << "Device Descriptor Data: " << info->deviceDescriptorData << "\n";
    return oss.str();
}
#endif

typedef HRESULT(*GameInputCreate_t)(IGameInput** gameInput);
static GameInputCreate_t TrueGameInputCreate = nullptr;
static IGameInput* gameInput = nullptr;
static IGameInputDevice* currentDevice = nullptr;

void CALLBACK OnDeviceConnectionChanged(
    _In_ GameInputCallbackToken callbackToken,
    _In_ void* context,
    _In_ IGameInputDevice* device,
    _In_ uint64_t timestamp,
    _In_ GameInputDeviceStatus currentStatus,
    _In_ GameInputDeviceStatus previousStatus) {
    LOG_FUNCTION_CALL;
    if (currentDevice == nullptr || device != currentDevice)
        return;

    // Device got disconnected
    if (!(currentStatus & GameInputDeviceStatus::GameInputDeviceConnected)) {
#ifdef VERBOSE
        log("Gamepad got disconnected");
#endif
        currentDevice = nullptr;
    }
}

IGameInputDevice* GetCurrentGamepad() {
    if (gameInput == nullptr) {
        TrueGameInputCreate(&gameInput);
        gameInput->RegisterDeviceCallback(
            nullptr,
            GameInputKind::GameInputKindGamepad,
            GameInputDeviceStatus::GameInputDeviceConnected,
            GameInputEnumerationKind::GameInputAsyncEnumeration,
            nullptr,
            OnDeviceConnectionChanged,
            nullptr);
    }

    if (currentDevice == nullptr) {
#ifdef VERBOSE
        log("Looking for new gamepad");
#endif
        IGameInputReading* reading;
        if (FAILED(gameInput->GetCurrentReading(GameInputKind::GameInputKindGamepad, nullptr, &reading)))
            return nullptr;
        reading->GetDevice(&currentDevice);
#ifdef VERBOSE
        log("Found new gamepad");
#endif
    }

    return currentDevice;
}

class GameInputDevice : public IGameInputDevice {
public:
    explicit GameInputDevice() {
        LOG_FUNCTION_CALL;
    }

    HRESULT QueryInterface(const IID& riid, void** ppvObj) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    ULONG AddRef() noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    ULONG Release() noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    const GameInputDeviceInfo* GetDeviceInfo() noexcept override {
        LOG_FUNCTION_CALL;

        static GameInputDeviceInfo dev_info = { 0 };
        if (dev_info.infoSize != 0) // Quickly return if already initialized
            return &dev_info;

        dev_info.infoSize = sizeof(GameInputDeviceInfo);

        dev_info.controllerAxisCount = 6;
        dev_info.controllerButtonCount = 14;

        dev_info.deviceId = EMU_DEVICE_ID;
        dev_info.deviceRootId = EMU_DEVICE_ID;

        dev_info.capabilities = GameInputDeviceCapabilityWireless;

        dev_info.vendorId = 0x045E; // Microsoft VID
        dev_info.productId = 0x0B13; // Wireless Xbox One PID

        // This is the source of the bug in the KCD2, which prevents rumble from working
        //  as it checks for some reason deviceFamily, because on gamepad connected via bluetooth it will report it as Xbox360 controller.
        dev_info.deviceFamily = GameInputDeviceFamily::GameInputFamilyXboxOne;
        dev_info.usage.id = 5;
        dev_info.usage.page = 1;

        dev_info.hardwareVersion.major = 1;
        dev_info.hardwareVersion.minor = 0;
        dev_info.firmwareVersion.major = 0;
        dev_info.firmwareVersion.minor = 0;

        dev_info.supportedInput = GameInputKindControllerAxis | GameInputKindControllerButton | GameInputKindGamepad | GameInputKindUiNavigation;
        dev_info.supportedRumbleMotors = GameInputRumbleLowFrequency | GameInputRumbleHighFrequency | GameInputRumbleLeftTrigger | GameInputRumbleRightTrigger;
        dev_info.supportedSystemButtons = GameInputSystemButtonGuide;

        static GameInputGamepadInfo gamepadInfo = {
            GameInputLabel::GameInputLabelXboxStart,
            GameInputLabel::GameInputLabelXboxBack,
            GameInputLabel::GameInputLabelXboxA,
            GameInputLabel::GameInputLabelXboxB,
            GameInputLabel::GameInputLabelXboxX,
            GameInputLabel::GameInputLabelXboxY,
            GameInputLabel::GameInputLabelXboxDPadUp,
            GameInputLabel::GameInputLabelXboxDPadDown,
            GameInputLabel::GameInputLabelXboxDPadLeft,
            GameInputLabel::GameInputLabelXboxDPadRight,
            GameInputLabel::GameInputLabelXboxLeftShoulder,
            GameInputLabel::GameInputLabelXboxRightShoulder,
            GameInputLabel::GameInputLabelXboxLeftStickButton,
            GameInputLabel::GameInputLabelXboxRightStickButton
        };
        dev_info.gamepadInfo = &gamepadInfo;
        dev_info.infoSize += sizeof(GameInputGamepadInfo);

        static GameInputUiNavigationInfo uiNavigationInfo = {
            GameInputLabel::GameInputLabelXboxStart,
            GameInputLabel::GameInputLabelXboxBack,
            GameInputLabel::GameInputLabelXboxA,
            GameInputLabel::GameInputLabelXboxB,
            GameInputLabel::GameInputLabelXboxDPadUp,
            GameInputLabel::GameInputLabelXboxDPadDown,
            GameInputLabel::GameInputLabelXboxDPadLeft,
            GameInputLabel::GameInputLabelXboxDPadRight,
            GameInputLabel::GameInputLabelXboxX,
            GameInputLabel::GameInputLabelXboxY,
            GameInputLabel::GameInputLabelXboxLeftStickButton,
            GameInputLabel::GameInputLabelXboxRightStickButton,
            GameInputLabel::GameInputLabelXboxLeftTrigger,
            GameInputLabel::GameInputLabelXboxRightTrigger,
            GameInputLabel::GameInputLabelXboxLeftShoulder,
            GameInputLabel::GameInputLabelXboxRightShoulder,
            (GameInputLabel)-1,
            (GameInputLabel)-1,
            (GameInputLabel)-1,
            (GameInputLabel)-1,
            (GameInputLabel)-1
        };
        dev_info.uiNavigationInfo = &uiNavigationInfo;
        dev_info.infoSize += sizeof(GameInputUiNavigationInfo);

        return &dev_info;
    }

    GameInputDeviceStatus GetDeviceStatus() noexcept override {
        LOG_FUNCTION_CALL;
        return (GetCurrentGamepad() == nullptr)
            ? GameInputDeviceStatus::GameInputDeviceNoStatus
            : GameInputDeviceStatus::GameInputDeviceConnected;
    }

    void GetBatteryState(GameInputBatteryState* state) noexcept override {
        LOG_FUNCTION_CALL;
    }

    HRESULT CreateForceFeedbackEffect(uint32_t motorIndex, const GameInputForceFeedbackParams* params, IGameInputForceFeedbackEffect** effect) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    bool IsForceFeedbackMotorPoweredOn(uint32_t motorIndex) noexcept override {
        LOG_FUNCTION_CALL;
        return false;
    }

    void SetForceFeedbackMotorGain(uint32_t motorIndex, float masterGain) noexcept override {
        LOG_FUNCTION_CALL;
    }

    HRESULT SetHapticMotorState(uint32_t motorIndex, const GameInputHapticFeedbackParams* params) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    void SetRumbleState(const GameInputRumbleParams* params) noexcept override {
        LOG_FUNCTION_CALL;
#ifdef VERBOSE
        sprintf(buffer, "Set Rumble State: [ Low Frequency: %.2f | High Frequency: %.2f | Left Trigger: %.2f | Right Trigger: %.2f ]",
            params->lowFrequency, params->highFrequency, params->leftTrigger, params->rightTrigger);
        log(buffer);
#endif

        IGameInputDevice* device = GetCurrentGamepad();
        if (device != nullptr)
            device->SetRumbleState(params);
    }

    void SetInputSynchronizationState(bool enabled) noexcept override {
        LOG_FUNCTION_CALL;
    }

    void SendInputSynchronizationHint() noexcept override {
        LOG_FUNCTION_CALL;
    }

    void PowerOff() noexcept override {
        LOG_FUNCTION_CALL;
    }

    HRESULT CreateRawDeviceReport(uint32_t reportId, GameInputRawDeviceReportKind reportKind, IGameInputRawDeviceReport** report) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT GetRawDeviceFeature(uint32_t reportId, IGameInputRawDeviceReport** report) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT SetRawDeviceFeature(IGameInputRawDeviceReport* report) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT SendRawDeviceOutput(IGameInputRawDeviceReport* report) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT SendRawDeviceOutputWithResponse(IGameInputRawDeviceReport* requestReport, IGameInputRawDeviceReport** responseReport) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT ExecuteRawDeviceIoControl(uint32_t controlCode, size_t inputBufferSize, const void* inputBuffer, size_t outputBufferSize, void* outputBuffer, size_t* outputSize) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    bool AcquireExclusiveRawDeviceAccess(uint64_t timeoutInMicroseconds) noexcept override {
        LOG_FUNCTION_CALL;
        return false;
    }

    void ReleaseExclusiveRawDeviceAccess() noexcept override {
        LOG_FUNCTION_CALL;
    }
};

class GameInputReading : public IGameInputReading {
private:
    uint64_t _timestamp = 0;

public:
    explicit GameInputReading() {
        LOG_FUNCTION_CALL;
    }

    HRESULT QueryInterface(const IID& riid, void** ppvObj) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    ULONG AddRef() noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    ULONG Release() noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    GameInputKind GetInputKind() noexcept override {
        LOG_FUNCTION_CALL;
        return GameInputKindGamepad;
    }

    uint64_t GetSequenceNumber(GameInputKind inputKind) noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    uint64_t GetTimestamp() noexcept override {
        LOG_FUNCTION_CALL;
        return _timestamp;
    }

    void GetDevice(IGameInputDevice** device) noexcept override {
        LOG_FUNCTION_CALL;
    }

    bool GetRawReport(IGameInputRawDeviceReport** report) noexcept override {
        LOG_FUNCTION_CALL;
        return false;
    }

    uint32_t GetControllerAxisCount() noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    uint32_t GetControllerAxisState(uint32_t stateArrayCount, float* stateArray) noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    uint32_t GetControllerButtonCount() noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    uint32_t GetControllerButtonState(uint32_t stateArrayCount, bool* stateArray) noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    uint32_t GetControllerSwitchCount() noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    uint32_t GetControllerSwitchState(uint32_t stateArrayCount, GameInputSwitchPosition* stateArray) noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    uint32_t GetKeyCount() noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    uint32_t GetKeyState(uint32_t stateArrayCount, GameInputKeyState* stateArray) noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    bool GetMouseState(GameInputMouseState* state) noexcept override {
        LOG_FUNCTION_CALL;
        return false;
    }

    uint32_t GetTouchCount() noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    uint32_t GetTouchState(uint32_t stateArrayCount, GameInputTouchState* stateArray) noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    bool GetMotionState(GameInputMotionState* state) noexcept override {
        LOG_FUNCTION_CALL;
        return false;
    }

    bool GetArcadeStickState(GameInputArcadeStickState* state) noexcept override {
        LOG_FUNCTION_CALL;
        return false;
    }

    bool GetFlightStickState(GameInputFlightStickState* state) noexcept override {
        LOG_FUNCTION_CALL;
        return false;
    }

    bool GetGamepadState(GameInputGamepadState* state) noexcept override {
        LOG_FUNCTION_CALL;

        IGameInputDevice* device = GetCurrentGamepad();
        if (device == nullptr)
            return false;

        IGameInputReading* reading;
        if (FAILED(gameInput->GetCurrentReading(GameInputKind::GameInputKindGamepad, device, &reading)))
            return false;

        _timestamp = reading->GetTimestamp();
        return reading->GetGamepadState(state);
    }

    bool GetRacingWheelState(GameInputRacingWheelState* state) noexcept override {
        LOG_FUNCTION_CALL;
        return false;
    }

    bool GetUiNavigationState(GameInputUiNavigationState* state) noexcept override {
        LOG_FUNCTION_CALL;
        return false;
    }
};

class GameInput : public IGameInput {
private:
    GameInputDevice _device{ };
    GameInputReading _reading{ };
    UINT64 _lastGamepadReading = 0;

public:
    HRESULT QueryInterface(const IID& riid, void** ppvObj) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    ULONG AddRef() noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    ULONG Release() noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    uint64_t GetCurrentTimestamp() noexcept override {
        LOG_FUNCTION_CALL;
        return 0;
    }

    HRESULT GetCurrentReading(GameInputKind inputKind, IGameInputDevice* device, IGameInputReading** reading) noexcept override {
        LOG_FUNCTION_CALL;

        if (device == &_device) {
            if (GetCurrentGamepad() == nullptr)
                return GAMEINPUT_E_DEVICE_DISCONNECTED;
            *reading = &_reading;
            return S_OK;
        }

        return E_NOTIMPL;
    }

    HRESULT GetNextReading(IGameInputReading* referenceReading, GameInputKind inputKind, IGameInputDevice* device, IGameInputReading** reading) noexcept override {
        LOG_FUNCTION_CALL;

        if (device == &_device) {
            GameInputGamepadState gamepad_state = { };
            if (_reading.GetGamepadState(&gamepad_state)) {
                if (std::exchange(_lastGamepadReading, _reading.GetTimestamp()) < _reading.GetTimestamp()) {
                    *reading = &_reading;
                    return S_OK;
                }
            }

            return GAMEINPUT_E_READING_NOT_FOUND;
        }

        return E_NOTIMPL;
    }

    HRESULT GetPreviousReading(IGameInputReading* referenceReading, GameInputKind inputKind, IGameInputDevice* device, IGameInputReading** reading) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT GetTemporalReading(uint64_t timestamp, IGameInputDevice* device, IGameInputReading** reading) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT RegisterReadingCallback(IGameInputDevice* device, GameInputKind inputKind, float analogThreshold, void* context, GameInputReadingCallback callbackFunc, GameInputCallbackToken* callbackToken) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT RegisterDeviceCallback(IGameInputDevice* device, GameInputKind inputKind, GameInputDeviceStatus statusFilter, GameInputEnumerationKind enumerationKind, void* context, GameInputDeviceCallback callbackFunc, GameInputCallbackToken* callbackToken) noexcept override {
        LOG_FUNCTION_CALL;

        if ((inputKind & GameInputKindGamepad) != 0 && (statusFilter & GameInputDeviceConnected) != 0) {
            LARGE_INTEGER timestamp;
            QueryPerformanceCounter(&timestamp);
            callbackFunc(callbackToken != nullptr ? *callbackToken : 0, context, &_device, timestamp.QuadPart, GameInputDeviceConnected, GameInputDeviceConnected);
            return S_OK;
        }
#ifdef VERBOSE
        log("Not returning fake device, inputKind: " + std::to_string(inputKind) + " statusFilter: " + std::to_string(statusFilter));
#endif
        return E_NOTIMPL;
    }

    HRESULT RegisterSystemButtonCallback(IGameInputDevice* device, GameInputSystemButtons buttonFilter, void* context, GameInputSystemButtonCallback callbackFunc, GameInputCallbackToken* callbackToken) {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT RegisterKeyboardLayoutCallback(IGameInputDevice* device, void* context, GameInputKeyboardLayoutCallback callbackFunc, GameInputCallbackToken* callbackToken) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    void StopCallback(GameInputCallbackToken callbackToken) noexcept override {
        LOG_FUNCTION_CALL;
    }

    bool UnregisterCallback(GameInputCallbackToken callbackToken, uint64_t timeoutInMicroseconds) noexcept override {
        LOG_FUNCTION_CALL;
        return false;
    }

    HRESULT CreateDispatcher(IGameInputDispatcher** dispatcher) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT CreateAggregateDevice(GameInputKind inputKind, IGameInputDevice** device) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT FindDeviceFromId(const APP_LOCAL_DEVICE_ID* value, IGameInputDevice** device) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT FindDeviceFromObject(IUnknown* value, IGameInputDevice** device) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT FindDeviceFromPlatformHandle(HANDLE value, IGameInputDevice** device) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT FindDeviceFromPlatformString(LPCWSTR value, IGameInputDevice** device) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    HRESULT EnableOemDeviceSupport(uint16_t vendorId, uint16_t productId, uint8_t interfaceNumber, uint8_t collectionNumber) noexcept override {
        LOG_FUNCTION_CALL;
        return E_NOTIMPL;
    }

    void SetFocusPolicy(GameInputFocusPolicy policy) noexcept override {
        LOG_FUNCTION_CALL;
    }
};

HRESULT HookedGameInputCreate(IGameInput** gameInput) {
#ifdef VERBOSE
    log("Called GameInputCreate");
#endif
    static GameInput gameInputSingleton{};
    *gameInput = &gameInputSingleton;
    return S_OK;
}

__declspec(dllexport) BOOL WINAPI DllMain(HINSTANCE hinst, DWORD dwReason, LPVOID reserved) {
    if (DetourIsHelperProcess()) {
        return TRUE;
    }

    LONG err;
    if (dwReason == DLL_PROCESS_ATTACH) {
#ifdef VERBOSE
        log("Detouring...");
#endif
        if (TrueGameInputCreate == nullptr) {
            TrueGameInputCreate = (GameInputCreate_t)DetourFindFunction("GameInput.dll", "GameInputCreate");
#ifdef VERBOSE
            sprintf(buffer, "TrueGameInputCreate: %p", TrueGameInputCreate);
            log(buffer);
#endif
        }

        if (!DetourRestoreAfterWith()) {
#ifdef VERBOSE
            log("DetourRestoreAfterWith failed");
#endif
            //return FALSE; // Can fail if not injected with withdll
        }

        err = DetourTransactionBegin();
        if (err != NO_ERROR) {
#ifdef VERBOSE
            sprintf(buffer, "DetourTransactionBegin failed: %X", err);
            log(buffer);
#endif
            return FALSE;
        }

        err = DetourUpdateThread(GetCurrentThread());
        if (err != NO_ERROR) {
#ifdef VERBOSE
            sprintf(buffer, "DetourUpdateThread failed: %X", err);
            log(buffer);
#endif
            return FALSE;
        }

        err = DetourAttach(&(PVOID&)TrueGameInputCreate, HookedGameInputCreate);
        if (err != NO_ERROR) {
#ifdef VERBOSE
            sprintf(buffer, "DetourAttach failed: %X", err);
            log(buffer);
#endif
            return FALSE;
        }

        err = DetourTransactionCommit();
        if (err != NO_ERROR) {
#ifdef VERBOSE
            sprintf(buffer, "DetourTransactionCommit failed: %X", err);
            log(buffer);
#endif
            return FALSE;
        }
#ifdef VERBOSE
        log("Hook attached");
#endif
    } else if (dwReason == DLL_PROCESS_DETACH) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourDetach(&(PVOID&)TrueGameInputCreate, HookedGameInputCreate);
        DetourTransactionCommit();
#ifdef VERBOSE
        log("Hook detached\n");
#endif
    }

    return TRUE;
}
