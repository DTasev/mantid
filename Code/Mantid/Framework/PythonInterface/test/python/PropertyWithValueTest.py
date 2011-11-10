import unittest

from mantid.api import algorithm_mgr

import numpy as np

class PropertyWithValueTest(unittest.TestCase):
    
    # Integration algorithm handle
    _integration = None
    # MaskDetectors algorithm handle
    _mask_dets = None
    
    def setUp(self):
        if self._integration is None:
            self.__class__._integration = algorithm_mgr.create_unmanaged("Integration")
            self.__class__._integration.initialize()
        if self._mask_dets is None:
            self.__class__._mask_dets = algorithm_mgr.create_unmanaged("MaskDetectors")
            self.__class__._mask_dets.initialize()
  
    def test_set_property_int(self):
        self._integration.set_property("StartWorkspaceIndex", 5) 
        self.assertEquals(self._integration.get_property("StartWorkspaceIndex").value, 5)

    def test_set_property_float(self):
        self._integration.set_property("RangeLower", 100.5)
        self.assertAlmostEqual(self._integration.get_property("RangeLower").value, 100.5)
        self._integration.set_property("RangeLower", 50) # Set with an int should still work
        self.assertAlmostEqual(self._integration.get_property("RangeLower").value, 50)

    def test_set_property_bool(self):
        self._integration.set_property("IncludePartialBins", True)
        self.assertEquals(self._integration.get_property("IncludePartialBins").value, True)

    def test_set_property_succeeds_with_python_int_lists(self):
        value = [2,3,4,5,6]
        self._mask_dets.set_property("WorkspaceIndexList", value) #size_t
        idx_list = self._mask_dets.get_property("WorkspaceIndexList").value
        self.assertEquals(len(idx_list), 5)
        for i in range(5):
            self.assertEquals(idx_list[i], i+2)
        value.append(7)
        
        self._mask_dets.set_property("DetectorList", value) #integer
        det_list = self._mask_dets.get_property("DetectorList").value 
        self.assertEquals(len(det_list), 6)
        for i in range(6):
            self.assertEquals(det_list[i], i+2)
            
    def test_set_property_succeeds_with_python_float_lists(self):
        rebin = algorithm_mgr.create_unmanaged("Rebin")
        rebin.initialize()
        input = [0.5,1.0,5.5]
        rebin.set_property('Params',input)
        params = rebin.get_property('Params').value
        self.assertEquals(len(params), 3)
        for i in range(3):
            self.assertEquals(params[i], input[i])
        
    def test_set_property_raises_type_error_when_a_list_contains_multiple_types(self):
        values = [2,3,4.0,5,6]
        self.assertRaises(TypeError, self._mask_dets.set_property, "WorkspaceIndexList", values) #size_t

    def _do_numpy_test(self, int_type=False):
        create_ws = algorithm_mgr.create_unmanaged('CreateWorkspace')
        create_ws.initialize()
        if int_type:
            datax = np.arange(10)
        else:
            datax = np.arange(10.0)
        create_ws.set_property('DataX', datax)
        x_values = create_ws.get_property('DataX').value
        self.assertEquals(len(x_values), 10)
        for i in range(10):
            self.assertEquals(x_values[i], i)	

    def test_set_property_succeeds_with_numpy_array_of_correct_type(self):
        self._do_numpy_test()

    def test_set_property_succeeds_with_numpy_array_of_correct_type(self):
        self._do_numpy_test(True)
