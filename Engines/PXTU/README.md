## Python Extended Test Unit (PXTU)

### Description
Python Extended Test Unit (PXTU) allows the user to interact with the signals present on the SBC side.

#### Capability
1. Can read from a particular signal.
2. Can write to a particular signal.
3. Can perform read \ write to a set of signals.
4. Can record multiple signals.

#### Limitations
No feature for pausing the recording. Should come in future implementations.

#### Building the Engine
To run the PXTU Engine, direct to the Falcon\Engine\PXTU and "import robot"

#### Operations
```
Reading from a signal:
>>> robot.base.usr.usr_left.value
6
>>> robot.base.usr.usr_right.value
5

Writing to a signal:
>>> robot.base.usr.usr_left.value = 1
'Success'
>>> robot.base.usr.usr_right.value = 1
'Success'

# Start the recording:
>>> robot.record.start([robot.base.usr.usr_left, robot.base.usr.usr_right])
>>> robot.record.start([robot.base.usr.usr_left, robot.base.usr.usr_right],100,"testing.csv")

# Stop recording:
>>> robot.record.stop()

# Multiple reads:
>>> robot.Signals.read([robot.base.usr.usr_left, robot.base.usr.usr_right])
["usr_left":2,"usr_right":5]

# Multiple writes:
>>> robot.Signals.write([robot.base.usr.usr_left, robot.base.usr.usr_right],[1,2])
```





