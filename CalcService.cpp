#include "CalcService.h"
#include <iostream>

CalcService::CalcService(
    shared_ptr<IResourceFactory> resource_factory, 
    shared_ptr<IServiceSettingsFactory> settings_factory) {
        _settings_factory = settings_factory;
        _service.publish(resource_factory->get_resource());
        cout << "Resource has been published" << endl;
    }

void CalcService::start() {
    _service.start(_settings_factory->get_settings());
}