include (externalProjectWrapper)

# nlohmann_json
add_external_project(json_external json "JSON_BuildTests=OFF")
add_external_target(nlohmann_json json/include json OBJECT json_external)