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

#### Reading signal
```
>>> robot.base.usr.usr_left.value
6

>>> robot.base.usr.usr_right.value
5
```



