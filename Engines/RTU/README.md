## Real Time Unit

### Description
Real Time Unit (RTU) is the engine responsible handling the communication with the SBC's shared memory. RTU only acts as a server and the user will not interact with the engine directly.
Shared memory will contain signals mapped at each address. RTU receives the command from PXTU engine and decodes the data later performs the action.
The data is packeted according to the format given in [.doc].

#### Capability
1. RTU can handle multiple, single read and writes.
2. RTU can perform simultaneous multiple operations with very least latency.

#### Limitations
No know limitations

#### Run Modes
Debug Mode: To run the engine in debug mode, the DEBUG switch in CMakeLists.txt should be set to 1. Doing this will launch the process with connect to the TTY console.
Real Mode: Making the DEBUG switch 0 while building, will make the program run as a daemon. The logging takes place in syslog.

#### Building the Engine
1. Direct to linux_build in RTU directory. if one doesn't exist, create it.
2. Type the following "$cmake .. && make". This builds the executable.
3. This executable can be run by "$./RTU".

#### Flowchart
The flowchart below gives a higher level structure of the source.

![Flowchart for RTU Engine](https://github.com/naveenspace7/falcon/blob/v0.3/Engines/RTU/doc/Flowchart.png)

