# ControlPlaneSmith: Generating Random Control-Plane Configurations from P4 Programs
ControlPlaneSmith is a [P4Tools](https://github.com/p4lang/p4c/tree/main/backends/p4tools) module to fuzz P4 control-planes. ControlPlaneSmith is intended to stress-test the control-plane interface of a P4 target. ControlPlaneSmith can also be used by other P4 tools to produce an initial control-plane configuration.


ControlPlaneSmith takes as argument a P4 program, the P4 architecture, and the P4 target and produces a random control-plane configuration specific to the input program. Currently, this configuration is specified in the [P4Runtime](https://github.com/p4lang/p4runtime) format in the form of a Protobuf Text Format file. In addition to this initial configuration, ControlPlaneSmith can also generate a timeseries of P4Runtime WriteRequests, which either insert a control-plane entry or update or delete an existing entry.


## Building ControlPlaneSmith

ControlPlaneSmith is a  [P4Tools](https://github.com/p4lang/p4c/tree/main/backends/p4tools) module, which itself is part of [P4C](https://github.com/p4lang/p4c). To be able to use ControlPlaneSmith, you first need to build P4C. Instructions can be found [here](https://github.com/p4lang/p4c#installing-p4c-from-source).

After building P4C, you need link ControlPlaneSmith as a P4Tools module. The [CI scripts](https://github.com/fruffy/rtsmith/blob/master/.github/workflows/ci-test.yml) describe a similar workflow.
You will need to symlink ControlPlaneSmith into the modules directory, rerun CMake in your P4C build directory, then rebuild.
```
ln -sf rtsmith p4c/backends/p4tools/modules/
cd p4c/build
cmake ..
make
```
