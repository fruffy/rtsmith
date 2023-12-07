# P4RuntimeSmith
P4RuntimeSmith is a [P4Tools](https://github.com/p4lang/p4c/tree/main/backends/p4tools) module to fuzz P4 control-planes. P4Runtime is intended to stress-test the control-plane interface of a P4 target. P4Runtime can also be used by other P4 tools to bootstrap an initial control-plane configuration.


P4RuntimeSmith takes as argument a P4 program, the P4 architecture, and the P4 target and produces a random control-plane configuration tailored to the input program. Currently, this configuration is specified in the [P4Runtime](https://github.com/p4lang/p4runtime) format in the format of a Protobuf Text Format file. In addition to this initial configuration, P4RuntimeSmith can also generate a timeseries of P4Runtime WriteRequests, which either insert a control-plane entry or update or delete an existing entry.


## Building P4RuntimeSmith

P4RuntimeSmith is a  [P4Tools](https://github.com/p4lang/p4c/tree/main/backends/p4tools) module, which itself is part of [P4C](https://github.com/p4lang/p4c). To be able to use P4RuntimeSmith, you first need to build P4C. Instructions can be found [here](https://github.com/p4lang/p4c#installing-p4c-from-source).

After building P4C, you need link P4RuntimeSmith as a P4Tools module. The [CI scripts](https://github.com/fruffy/p4rtsmith/blob/master/.github/workflows/ci-test.yml) describe a similar workflow.
You will need to symlink P4RuntimeSmith into the modules directory, rerun CMake in your P4C build directory, then rebuild.
```
ln -sf p4rtsmith p4c/backends/p4tools/modules/
cd p4c/build
cmake ..
make
```
