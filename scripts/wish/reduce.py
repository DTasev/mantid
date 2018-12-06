from __future__ import (absolute_import, division, print_function)
import mantid.simpleapi as mantid
import os
import numpy as n


class Wish:
    NUM_PANELS = 11
    NUM_MONITORS = 6
    LAMBDA_RANGE = (0.7, 10.35)

    def __init__(self, cal_directory, output_folder, delete_ws,
                 user_directory="/archive/ndxwish/Instrument/data/cycle_", absorb=True):
        self.absorb = absorb
        self.cal_dir = cal_directory
        self.use_folder = user_directory
        self.out_folder = output_folder
        self.deleteWorkspace = delete_ws
        self.is_vanadium = False
        self.username = None
        self.data_directory = None
        self.user_directory = None
        self.datafile = None
        self.userdatadir = None
        self.userdataprocessed = None
        self.return_panel = {
            1: (6, 19461),
            2: (19462, 38917),
            3: (38918, 58373),
            4: (58374, 77829),
            5: (77830, 97285),
            6: (97286, 116741),
            7: (116742, 136197),
            8: (136198, 155653),
            9: (155654, 175109),
            10: (175110, 194565),
            0: (6, 194565)
        }
        self.return_panel_van = {
            1: (6, 19461),
            2: (19462, 38917),
            3: (38918, 58373),
            4: (58374, 77829),
            5: (77830, 97285),
            6: (77830, 97285),
            7: (58374, 77829),
            8: (38918, 58373),
            9: (19462, 38917),
            10: (6, 19461)
        }
        # update to reflect new cycles
        self.cycle_mapping = [("09_2", (586, 774)), ("09_3", (774, 1728)), ("09_4", (1728, 3334)),
                              ("09_5", (3334, 16777)), ("10_1", (16777, 17322)), ("10_2", (17322, 17339)),
                              ("10_3", (17340, 17692)), ("11_1", (17692, 18051)), ("11_2", (18053, 18572)),
                              ("11_3", (18572, 19608)), ("11_4", (19608, 20420)), ("11_5", (20420, 21111)),
                              ("12_1", (21117, 21919)), ("12_2", (21919, 22437)), ("12_3", (22438, 23128)),
                              ("12_4", (23128, 23548)), ("12_5", (23548, 24086)), ("13_1", (24086, 24819)),
                              ("13_2", (24820, 25317)), ("13_3", (25318, 26148)), ("13_4", (26149, 26661)),
                              ("13_5", (26661, 27805)), ("14_1", (27806, 28904)), ("14_2", (28904, 29587)),
                              ("14_3", (29587, 30528)), ("15_1", (30528, 31669)), ("15_2", (31669, 32520)),
                              ("15_3", (32520, 33278)), ("15_4", (33278, 34171)), ("16_1", (34171, 35222)),
                              ("16_2", (35222, 35830)), ("16_3", (35830, 36442)), ("16_4", (36442, 37035)),
                              ("16_5", (37035, 38247)), ("17_1", (38247, 38837)), ("17_2", (38837, 39326)),
                              ("17_4", (39326, 40469)), ("18_1", (40469, 41846)), ("18_2", (41846, 42708)),
                              ("18_3", (42708, 43124))]

    def set_user_name(self, username):
        self.username = username

    def set_data_directory(self, directory="/archive/ndxwish/Instrument/data/cycle_09_5/"):
        self.data_directory = directory

    def set_user_directory(self, directory):
        self.user_directory = directory

    def startup(self, cycle='14_3'):
        user_data_directory = os.path.normpath(self.use_folder + cycle)
        self.set_data_directory(user_data_directory)
        print("Raw Data in :   ", user_data_directory)
        user_data_processed = self.out_folder
        self.set_user_directory(directory=user_data_processed)
        print("Processed Data in :   ", user_data_processed)

    def get_cycle(self, run_number):
        for cycle_pair in self.cycle_mapping:
            if run_number in range(cycle_pair[1][0], cycle_pair[1][1]):
                return cycle_pair[0]
        print("Failed to find cycle")

    # Returns the calibration filename
    def get_cal(self):
        return os.path.join(self.cal_dir, "WISH_cycle_15_4_noends_10to10_dodgytube_removed_feb2016.cal")

    # Returns the grouping filename
    def get_group_file(self):
        return os.path.join(self.cal_dir, "WISH_cycle_15_4_noends_10to10_dodgytube_removed_feb2016.cal")

    def get_vanadium(self, panel, cycle="09_4"):
        vanadium_string = {
            "09_2": "vana318-{}foc-rmbins-smooth50.nx5",
            "09_3": "vana935-{}foc-SS.nx5",
            "09_4": "vana3123-{}foc-SS.nx5",
            "09_5": "vana3123-{}foc-SS.nx5",
            "11_1": "vana17718-{}foc-SS.nxs",
            "11_2": "vana16812-{}foc-SS.nx5",
            "11_3": "vana18590-{}foc-SS-new.nxs",
            "11_4": "vana19612-{}foc-SF-SS.nxs",
        }
        return os.path.join(self.cal_dir, vanadium_string.get(cycle).format(panel))

    def get_file_name(self, run_number, extension):
        if extension[0] != 's':
            data_dir = self.data_directory
        else:
            data_dir = self.data_directory
        digit = len(str(run_number))
        filename = os.path.join(data_dir, "WISH")
        for i in range(8 - digit):
            filename = filename + "0"
        filename += str(run_number) + "." + extension
        return filename

    # Reads a wish data file return a workspace with a short name
    def read(self, number, panel, extension):
        if type(number) is int:
            filename = self.datafile
            print("will be reading filename...", filename)
            spectra_min, spectra_max = self.return_panel_van.get(panel) if self.is_vanadium else \
                self.return_panel.get(panel)
            if panel != 0:
                output = "w{0}-{1}".format(number, panel)
            else:
                output = "w{}".format(number)
            shared_load_files(extension, filename, output, spectra_max, spectra_min, False)
            if extension == "nxs_event":
                mantid.LoadEventNexus(Filename=filename, OutputWorkspace=output, LoadMonitors='1')
                self.read_event_nexus(number, output, panel)
            if extension[:10] == "nxs_event_":
                label, tmin, tmax = split_string_event(extension)
                output = output + "_" + label
                if tmax == "end":
                    mantid.LoadEventNexus(Filename=filename, OutputWorkspace=output, FilterByTimeStart=tmin,
                                          LoadMonitors='1', MonitorsAsEvents='1', FilterMonByTimeStart=tmin)
                else:
                    mantid.LoadEventNexus(Filename=filename, OutputWorkspace=output, FilterByTimeStart=tmin,
                                          FilterByTimeStop=tmax, LoadMonitors='1', MonitorsAsEvents='1',
                                          FilterMonByTimeStart=tmin, FilterMonByTimeStop=tmax)
                self.read_event_nexus(number, output, panel)
        else:
            num_1, num_2 = split_string(number)
            output = "w{0}_{1}-{2}".format(num_1, num_2, panel)
            output1 = self.load_multi_run_part(extension, num_1, panel)
            output2 = self.load_multi_run_part(extension, num_2, panel)
            mantid.MergeRuns(output1 + "," + output2, output)
            mantid.DeleteWorkspace(output1)
            mantid.DeleteWorkspace(output2)
        mantid.ConvertUnits(InputWorkspace=output, OutputWorkspace=output, Target="Wavelength", Emode="Elastic")
        lmin, lmax = Wish.LAMBDA_RANGE
        mantid.CropWorkspace(InputWorkspace=output, OutputWorkspace=output, XMin=lmin, XMax=lmax)
        monitor_run = "monitor{}".format(number)
        if monitor_run not in mantid.mtd:
            monitor = self.process_incidentmon(number, extension, spline_terms=70)
        else:
            monitor = mantid.mtd[monitor_run]
        mantid.NormaliseToMonitor(InputWorkspace=output, OutputWorkspace=output + "norm1", MonitorWorkspace=monitor)
        mantid.NormaliseToMonitor(InputWorkspace=output + "norm1", OutputWorkspace=output + "norm2",
                                  MonitorWorkspace=monitor, IntegrationRangeMin=0.7, IntegrationRangeMax=10.35)
        mantid.DeleteWorkspace(output)
        mantid.DeleteWorkspace(output + "norm1")
        mantid.RenameWorkspace(InputWorkspace=output + "norm2", OutputWorkspace=output)
        mantid.ConvertUnits(InputWorkspace=output, OutputWorkspace=output, Target="TOF", EMode="Elastic")
        mantid.ReplaceSpecialValues(InputWorkspace=output, OutputWorkspace=output, NaNValue=0.0, NaNError=0.0,
                                    InfinityValue=0.0, InfinityError=0.0)
        return output

    def load_multi_run_part(self, extension, run, panel):
        filename = self.get_file_name(run, extension)
        print("reading filename...", filename)
        spectra_min, spectra_max = self.return_panel.get(panel)
        output1 = "w{0}-{1}".format(run, panel)
        mantid.LoadRaw(Filename=filename, OutputWorkspace=output1, SpectrumMin=str(spectra_min),
                       SpectrumMax=str(spectra_max), LoadLogFiles="0")
        return output1

    def read_event_nexus(self, number, output, panel):
        mantid.RenameWorkspace("{}_monitors".format(output), "w{}_monitors".format(number))
        mantid.Rebin(InputWorkspace=output, OutputWorkspace=output, Params='6000,-0.00063,110000')
        mantid.ConvertToMatrixWorkspace(output, output)
        spectra_min, spectra_max = self.return_panel.get(panel)
        mantid.CropWorkspace(InputWorkspace=output, OutputWorkspace=output, StartWorkspaceIndex=spectra_min - 6,
                             EndWorkspaceIndex=spectra_max - 6)
        mantid.MaskBins(InputWorkspace=output, OutputWorkspace=output, XMin=99900, XMax=106000)

    # Focus dataset for a given panel and return the workspace
    def focus_onepanel(self, work, focus, panel):
        cal = "WISH_diff{}"
        if cal.format("_cal") not in mantid.mtd:
            mantid.LoadDiffCal(filename=self.get_cal(), InstrumentName="WISH", WorkspaceName=cal.format(""))
        mantid.AlignDetectors(InputWorkspace=work, OutputWorkspace=work, CalibrationWorkspace=cal.format("_cal"))
        mantid.DiffractionFocussing(InputWorkspace=work, OutputWorkspace=focus, GroupingWorkspace=cal.format("_group"))
        if self.deleteWorkspace:
            mantid.DeleteWorkspace(work)
        if panel == 5 or panel == 6:
            mantid.CropWorkspace(InputWorkspace=focus, OutputWorkspace=focus, XMin=0.3)
        return focus

    def focus(self, work, panel):
        focus = "{}foc".format(work)
        if panel != 0:
            return self.focus_onepanel(work, focus, panel)
        else:
            self.focus_onepanel(work, focus, panel)
            split(focus)

    def process_run(self, number, panel, extension, cycle_vana="09_4", absorb=False, number_density=0.0, scattering=0.0,
                    attenuation=0.0, height=0.0, radius=0.0):
        ws_to_focus = self.read(number, panel, extension)
        if absorb:
            absorption_corrections(attenuation, height, number_density, radius, scattering, ws_to_focus)
        focused_ws = self.focus(ws_to_focus, panel)

        panel_crop = {
            1: (0.8, 53.3),
            2: (0.5, 13.1),
            3: (0.5, 7.77),
            4: (0.4, 5.86),
            5: (0.35, 4.99),
            6: (0.35, 4.99),
            7: (0.4, 5.86),
            8: (0.5, 7.77),
            9: (0.5, 13.1),
            10: (0.8, 53.3)
        }
        d_min, d_max = panel_crop.get(panel)
        mantid.CropWorkspace(InputWorkspace=focused_ws, OutputWorkspace=focused_ws, XMin=d_min, XMax=d_max)
        save_location = os.path.join(self.user_directory, "{0}-{1}{2}.{3}")
        if panel == 0:
            for panel_i in range(Wish.NUM_PANELS):
                focused_ws = "w{0}-{1}foc".format(number, panel_i)
                mantid.CropWorkspace(InputWorkspace=focused_ws, OutputWorkspace=focused_ws, XMin=d_min, XMax=d_max)
                print("will try to load a vanadium with the name:", self.get_vanadium(panel_i, cycle_vana))
                self.apply_vanadium_corrections(cycle_vana, panel_i, focused_ws)
                mantid.SaveGSS(InputWorkspace=focused_ws,
                               Filename=save_location.format(number, panel_i, extension, "gss"),
                               Append=False, Bank=1)
                mantid.SaveFocusedXYE(focused_ws, save_location.format(number, panel_i, extension, "dat"))
                mantid.SaveNexusProcessed(focused_ws, save_location.format(number, panel_i, extension, "nxs"))
        else:
            print("will try to load a vanadium with the name:", self.get_vanadium(panel, cycle_vana))
            self.apply_vanadium_corrections(cycle_vana, panel, focused_ws)
            mantid.SaveGSS(InputWorkspace=focused_ws,
                           Filename=save_location.format(number, panel, extension, "gss"),
                           Append=False, Bank=1)
            mantid.SaveFocusedXYE(focused_ws, save_location.format(number, panel, extension, "dat"))
            mantid.SaveNexusProcessed(focused_ws, save_location.format(number, panel, extension, "nxs"))
        return focused_ws

    def apply_vanadium_corrections(self, cyclevana, i, focused_ws):
        mantid.LoadNexusProcessed(Filename=self.get_vanadium(i, cyclevana), OutputWorkspace="vana")
        mantid.RebinToWorkspace(WorkspaceToRebin="vana", WorkspaceToMatch=focused_ws, OutputWorkspace="vana")
        mantid.Divide(LHSWorkspace=focused_ws, RHSWorkspace="vana", OutputWorkspace=focused_ws)
        mantid.DeleteWorkspace("vana")
        mantid.ConvertUnits(InputWorkspace=focused_ws, OutputWorkspace=focused_ws, Target="TOF",
                            EMode="Elastic")
        mantid.ReplaceSpecialValues(InputWorkspace=focused_ws, OutputWorkspace=focused_ws, NaNValue=0.0,
                                    NaNError=0.0,
                                    InfinityValue=0.0, InfinityError=0.0)

    # Create a corrected vanadium (normalise,corrected for attenuation and empty, strip peaks) and
    # save a a nexus processed file.
    # It looks like smoothing of 100 works quite well
    def process_vanadium(self, vanadium, empty, panel, height, radius, cycle_van="09_3", cycle_empty="09_3"):
        user_data_directory = self.use_folder + cycle_van + '/'
        self.set_data_directory(user_data_directory)
        self.datafile = self.get_file_name(vanadium, "raw")
        vanadium_ws = self.read(vanadium, panel, "raw")
        user_data_directory = self.use_folder + cycle_empty + '/'
        self.set_data_directory(user_data_directory)
        self.datafile = self.get_file_name(empty, "raw")
        empty_ws = self.read(empty, panel, "raw")
        mantid.Minus(LHSWorkspace=vanadium_ws, RHSWorkspace=empty_ws, OutputWorkspace=vanadium_ws)
        mantid.DeleteWorkspace(empty_ws)
        absorption_corrections(4.8756, height, 0.07118, radius, 5.16, vanadium_ws)
        vanfoc = self.focus(vanadium_ws, panel)

        panel_crop = {
            1: (0.95, 53.3),
            2: (0.58, 13.1),
            3: (0.44, 7.77),
            4: (0.38, 5.86),
            5: (0.35, 4.99),
            6: (0.35, 4.99),
            7: (0.38, 5.86),
            8: (0.44, 7.77),
            9: (0.58, 13.1),
            10: (0.95, 53.3)
        }
        d_min, d_max = panel_crop.get(panel)
        mantid.CropWorkspace(InputWorkspace=vanfoc, OutputWorkspace=vanfoc, XMin=d_min, XMax=d_max)
        spline_coefficient = {
            1: 120,
            2: 120,
            3: 120,
            4: 130,
            5: 140,
            6: 140,
            7: 130,
            8: 120,
            9: 120,
            10: 120
        }
        mantid.SplineBackground(InputWorkspace=vanfoc, OutputWorkspace=vanfoc, NCoeff=spline_coefficient.get(panel))
        smoothing_coefficient = "30" if panel == 3 else "40"
        mantid.SmoothData(InputWorkspace=vanfoc, OutputWorkspace=vanfoc, NPoints=smoothing_coefficient)
        return

    def create_vanadium_run(self, van_run_number, empty_run_number, panels):
        self.is_vanadium = True
        vanadium_cycle = self.get_cycle(van_run_number)
        empty_cycle = self.get_cycle(empty_run_number)
        for panel in panels:
            self.process_vanadium(van_run_number, empty_run_number, panel, 4, 0.14999999999999999, vanadium_cycle,
                                  empty_cycle)
        monitor_runs = "monitor{}"
        mantid.DeleteWorkspace(monitor_runs.format(van_run_number))
        mantid.DeleteWorkspace(monitor_runs.format(empty_run_number))
        mantid.DeleteWorkspace("WISH_diff_cal")
        mantid.DeleteWorkspace("WISH_diff_group")
        mantid.DeleteWorkspace("WISH_diff_mask")

    def process_incidentmon(self, number, extension, spline_terms=20):
        if type(number) is int:
            filename = self.get_file_name(number, extension)
            works = "monitor{}".format(number)
            shared_load_files(extension, filename, works, 4, 4, True)
            if extension[:9] == "nxs_event":
                temp = "w{}_monitors".format(number)
                works = "w{}_monitor4".format(number)
                mantid.Rebin(InputWorkspace=temp, OutputWorkspace=temp, Params='6000,-0.00063,110000',
                             PreserveEvents=False)
                mantid.ExtractSingleSpectrum(InputWorkspace=temp, OutputWorkspace=works, WorkspaceIndex=3)
        else:
            num_1, num_2 = split_string(number)
            works = "monitor{0}_{1}".format(num_1, num_2)
            filename = self.get_file_name(num_1, extension)
            works1 = "monitor{}".format(num_1)
            mantid.LoadRaw(Filename=filename, OutputWorkspace=works1, SpectrumMin=4, SpectrumMax=4, LoadLogFiles="0")
            filename = self.get_file_name(num_2, extension)
            works2 = "monitor{}".format(num_2)
            mantid.LoadRaw(Filename=filename, OutputWorkspace=works2, SpectrumMin=4, SpectrumMax=4, LoadLogFiles="0")
            mantid.MergeRuns(InputWorkspaces=works1 + "," + works2, OutputWorkspace=works)
            mantid.DeleteWorkspace(works1)
            mantid.DeleteWorkspace(works2)
        mantid.ConvertUnits(InputWorkspace=works, OutputWorkspace=works, Target="Wavelength", Emode="Elastic")
        lambda_min, lambda_max = Wish.LAMBDA_RANGE
        mantid.CropWorkspace(InputWorkspace=works, OutputWorkspace=works, XMin=lambda_min, XMax=lambda_max)
        ex_regions = n.array([[4.57, 4.76],
                              [3.87, 4.12],
                              [2.75, 2.91],
                              [2.24, 2.50]])
        mantid.ConvertToDistribution(works)

        for reg in range(0, 4):
            mantid.MaskBins(InputWorkspace=works, OutputWorkspace=works, XMin=ex_regions[reg, 0],
                            XMax=ex_regions[reg, 1])

        mantid.SplineBackground(InputWorkspace=works, OutputWorkspace=works, WorkspaceIndex=0, NCoeff=spline_terms)

        mantid.SmoothData(InputWorkspace=works, OutputWorkspace=works, NPoints=40)
        mantid.ConvertFromDistribution(works)
        return works

    def reduce(self, run_numbers, panels):
        self.is_vanadium = False
        for run in run_numbers:
            self.startup(self.get_cycle(run))
            self.datafile = self.get_file_name(run, "raw")
            for panel in panels:
                wout = self.process_run(run, panel, "raw", "11_4", absorb=self.absorb, number_density=0.025,
                                        scattering=5.463, attenuation=2.595, height=4.0, radius=0.55)
                mantid.ConvertUnits(InputWorkspace=wout, OutputWorkspace="{}-d".format(wout), Target="dSpacing",
                                    EMode="Elastic")

            mantid.DeleteWorkspace("WISH_diff_cal")
            mantid.DeleteWorkspace("WISH_diff_group")
            mantid.DeleteWorkspace("WISH_diff_mask")
            mantid.DeleteWorkspace("monitor{}".format(run))
            if 0 in panels:
                panels = [1, 2, 3, 4, 5]
            for panel in [x for x in panels if x < 6]:
                self.save_combined_panel(run, panel)

    def save_combined_panel(self, run, panel):
        panel_combination = {
            5: 6,
            4: 7,
            3: 8,
            2: 9,
            1: 10
        }
        input_ws1 = "w{0}-{1}foc".format(run, panel)
        input_ws2 = "w{0}-{1}foc".format(run, panel_combination.get(panel))
        combined = "{0}{1}-{2}_{3}foc{4}".format("{0}", run, panel, panel_combination.get(panel), "{1}")
        combined_save = combined.format("", "{}")
        combined_ws = combined.format("w", "")

        mantid.RebinToWorkspace(WorkspaceToRebin=input_ws2, WorkspaceToMatch=input_ws1, OutputWorkspace=input_ws2,
                                PreserveEvents='0')
        mantid.Plus(LHSWorkspace=input_ws1, RHSWorkspace=input_ws2, OutputWorkspace=combined_ws)
        mantid.ConvertUnits(InputWorkspace=combined_ws, OutputWorkspace=combined_ws + "-d", Target="dSpacing",
                            EMode="Elastic")

        mantid.SaveGSS(combined_ws, os.path.join(self.user_directory, combined_save.format("raw.gss")), Append=False,
                       Bank=1)
        mantid.SaveFocusedXYE(combined_ws, os.path.join(self.user_directory, combined_save.format("raw.dat")))
        mantid.SaveNexusProcessed(combined_ws, os.path.join(self.user_directory, combined_save.format("raw.nxs")))


def absorption_corrections(attenuation, height, number_density, radius, scattering, input_ws):
    mantid.ConvertUnits(InputWorkspace=input_ws, OutputWorkspace=input_ws, Target="Wavelength", EMode="Elastic")
    mantid.CylinderAbsorption(InputWorkspace=input_ws, OutputWorkspace="absorptionWS",
                              CylinderSampleHeight=height, CylinderSampleRadius=radius,
                              AttenuationXSection=attenuation, ScatteringXSection=scattering,
                              SampleNumberDensity=number_density, NumberOfSlices="10", NumberOfAnnuli="10",
                              NumberOfWavelengthPoints="25", ExpMethod="Normal")
    mantid.Divide(LHSWorkspace=input_ws, RHSWorkspace="absorptionWS", OutputWorkspace=input_ws)
    mantid.DeleteWorkspace("absorptionWS")
    mantid.ConvertUnits(InputWorkspace=input_ws, OutputWorkspace=input_ws, Target="TOF", EMode="Elastic")


def split(focus):
    for i in range(Wish.NUM_PANELS):
        out = "{0}-{1}foc".format(focus[0:len(focus) - 3], i + 1)
        mantid.ExtractSingleSpectrum(InputWorkspace=focus, OutputWorkspace=out, WorkspaceIndex=i)
        mantid.DeleteWorkspace(focus)


def split_string(string):
    indxp = 0
    for i in range(0, len(string)):
        if string[i] == "+":
            indxp = i
    if indxp != 0:
        return int(string[0:indxp]), int(string[indxp + 1:len(string)])


def split_string_event(input_string):
    # this assumes the form nxs_event_label_tmin_tmax
    section = input_string.split('_')
    label = section[2]
    t_min = section[3]
    t_max = section[4]
    return label, t_min, t_max


def shared_load_files(extension, filename, ws, spectrum_max, spectrum_min, is_monitor):
    if not (extension == "nxs" or extension == "raw" or extension[0] == "s"):
        return False
    if extension == "nxs":
        mantid.Load(Filename=filename, OutputWorkspace=ws, SpectrumMin=spectrum_min, SpectrumMax=spectrum_min)
    else:
        mantid.LoadRaw(Filename=filename, OutputWorkspace=ws, SpectrumMin=spectrum_min, SpectrumMax=spectrum_max,
                       LoadLogFiles="0")
    mantid.Rebin(InputWorkspace=ws, OutputWorkspace=ws, Params='6000,-0.00063,110000')
    if not is_monitor:
        mantid.MaskBins(InputWorkspace=ws, OutputWorkspace=ws, XMin=99900, XMax=106000)
    return True
