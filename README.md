# n-ftdie n-ftdi Extended
FTDI bindings for N-API Node.js, with the same interface as the managed .NET wrapper.

Add support to ChipID and build it to Node NW.

Make sure your Node version is compatible with the NW

### Install Visual Studio Build Tools
 $ npm install --global windows-build-tools

### install ng-gyp
npm install -g nw-gyp

PS: Save the path shown after install, you will need it for: npm_config_node_gyp

### Set the NPM config, remember to change to your version of NW
- set npm_config_target=0.36.4
- set npm_config_arch=ia32
- set npm_config_target_arch=ia32
- set npm_config_runtime=node-webkit
- set npm_config_build_from_source=true
- set npm_config_node_gyp=...\nw-gyp.js 

### To build:
npm install n-ftdie
 
