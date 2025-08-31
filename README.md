# KBus Web

HTTP Access to WAGO KBus.

> [!NOTE]
> Tested with PFC G2 firmware FW28 (04.06.01).

## Usage

* Setup PFC G2 SDK
* Copy contents of [rules](rules) directory into `ptxproj/rules` directory of PFC G2 SDK
* Create a new directory `ptxproj/local_src/kbusweb` in the PFC G2 SDK
* Copy contents of this repository into the newly created directory
* Use `ptxdist bash` to enter PFC G2 SDK's development environment
* Run `ptxdist menuconfig` and activate the new entry `kbusweb`
* Run `ptxdist targetinstall kbusweb` to create the packages
* Find the IPK file in the ptxdist/platform-wago-pfcXXX/packages directory and install it
* Disable CODESYS
* Run `kbusweb` with root privileges on the device
* Open a browser and connect to HTTP port 8080 on the device

## Resources

* [WAGO PFC G2 SDK](https://github.com/WAGO/pfc-firmware-sdk-G2)
* [WAGO HowTo ADI MyKBusApplication](https://github.com/WAGO/pfc-howtos/tree/master/HowTo_ADI-MyKBusApplikation)
