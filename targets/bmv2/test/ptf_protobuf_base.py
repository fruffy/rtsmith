from functools import wraps
from typing import Any, List

import google.protobuf.text_format
import grpc
from p4.config.v1 import p4info_pb2
from p4.v1 import p4runtime_pb2, p4runtime_pb2_grpc
from ptf import config
from ptf import testutils as ptfutils
from ptf.base_tests import BaseTest
from ptf.mask import Mask

from tools import testutils
from tools.ptf import base_test as bt


class AbstractTest(bt.P4RuntimeTest):
    @bt.autocleanup
    def setUp(self):
        bt.P4RuntimeTest.setUp(self)
        success = bt.P4RuntimeTest.updateConfig(self)
        assert success

    def tearDown(self):
        bt.P4RuntimeTest.tearDown(self)

    def setupCtrlPlane(self) -> None:
        pass

    def sendCtrlPlaneUpdate(self) -> None:
        pass

    def runTestImpl(self) -> None:
        testutils.log.info("Configuring control plane...")
        self.setupCtrlPlane()

        testutils.log.info("Sending control plane updates...")
        self.sendCtrlPlaneUpdate()


class SetUpControlPlaneTest(AbstractTest):
    def runTest(self):
        self.runTestImpl()
