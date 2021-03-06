#include "ft_get_device_info_detail_op.h"

Napi::Object FtGetDeviceInfoDetailOp::Init(Napi::Env env, Napi::Object exports)
{
    exports.Set("getDeviceInfoDetailSync", Napi::Function::New(env, FtGetDeviceInfoDetailOp::InvokeSync));
    exports.Set("getDeviceInfoDetail", Napi::Function::New(env, FtGetDeviceInfoDetailOp::Invoke));
    return exports;
}

Napi::Object FtGetDeviceInfoDetailOp::InvokeSync(const Napi::CallbackInfo &info)
{
    DWORD index = info[0].As<Napi::Number>().Int32Value();
    DWORD flags;
    DWORD id;
    DWORD type;
    DWORD locId;
    char serialNumber[16];
    char description[64];
    unsigned long chipId;
    long comPortNumber;
    FT_HANDLE ftHandle;
    FT_STATUS ftStatus = FT_GetDeviceInfoDetail(index, &flags, &type, &id, &locId, serialNumber, description, &ftHandle);
    
    FTID_GetDeviceChipID(index, &chipId);

    if (FT_Open(index, &ftHandle) == FT_OK) {
        FT_GetComPortNumber(ftHandle, &comPortNumber);
        FT_Close(ftHandle);
    }
    

    return CreateResult(info.Env(), ftStatus, flags, id, type, locId, serialNumber, description, chipId, comPortNumber, ftHandle);
}

Napi::Promise FtGetDeviceInfoDetailOp::Invoke(const Napi::CallbackInfo &info)
{
    DWORD index = info[0].As<Napi::Number>().Int32Value();
    auto *operation = new FtGetDeviceInfoDetailOp(info.Env(), index);
    operation->Queue();
    return operation->Promise();
}

FtGetDeviceInfoDetailOp::FtGetDeviceInfoDetailOp(Napi::Env env, DWORD index) : FtBaseOp(env), index(index) {}

void FtGetDeviceInfoDetailOp::Execute()
{
    ftStatus = FT_GetDeviceInfoDetail(index, &flags, &type, &id, &locId, serialNumber, description, &ftHandle);
    FTID_GetDeviceChipID(index, &chipId);

    if (FT_Open(index, &ftHandle) == FT_OK) {
        FT_GetComPortNumber(ftHandle, &comPortNumber);
        FT_Close(ftHandle);
    }
}

void FtGetDeviceInfoDetailOp::OnOK()
{
    Napi::HandleScope scope(Env());
    deferred.Resolve(CreateResult(Env(), ftStatus, flags, id, type, locId, serialNumber, description, chipId, comPortNumber, ftHandle));
}

Napi::Object FtGetDeviceInfoDetailOp::CreateResult(
    Napi::Env env,
    FT_STATUS ftStatus,
    DWORD flags,
    DWORD id,
    DWORD type,
    DWORD locId,
    char *serialNumber,
    char *description,
    DWORD chipId, 
    long comPortNumber,
    FT_HANDLE ftHandle)
{
    Napi::Object result = Napi::Object::New(env);
    result.Set("ftStatus", ftStatus);
    Napi::Object deviceInfoNode = Napi::Object::New(env);
    deviceInfoNode.Set("flags", flags);
    deviceInfoNode.Set("type", type);
    deviceInfoNode.Set("id", id);
    deviceInfoNode.Set("locId", locId);
    deviceInfoNode.Set("serialNumber", serialNumber);
    deviceInfoNode.Set("description", description);
    deviceInfoNode.Set("COM", comPortNumber);
    deviceInfoNode.Set("chipID", chipId);
    if (ftHandle != nullptr)
    {
        deviceInfoNode.Set("ftHandle", Napi::External<void>::New(env, ftHandle));
    }
    result.Set("deviceInfoNode", deviceInfoNode);
    return result;
}