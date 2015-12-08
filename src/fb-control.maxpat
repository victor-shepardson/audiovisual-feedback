{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 7,
			"minor" : 0,
			"revision" : 5,
			"architecture" : "x86",
			"modernui" : 1
		}
,
		"rect" : [ 715.0, 116.0, 559.0, 525.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 0,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"boxes" : [ 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-96",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 104.0, 299.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-97",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 104.0, 327.0, 87.0, 22.0 ],
					"style" : "",
					"text" : "test_param $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-94",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 3.0, 145.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-95",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 3.0, 173.0, 69.0, 22.0 ],
					"style" : "",
					"text" : "lf_bleed $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-85",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"patching_rect" : [ 391.75, 354.0, 35.0, 22.0 ],
					"style" : "",
					"text" : "* 0.5"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"ft1" : 5.0,
					"id" : "obj-83",
					"maxclass" : "number~",
					"mode" : 2,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 362.5, 330.0, 56.0, 22.0 ],
					"sig" : 0.0,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-84",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 362.5, 306.0, 65.0, 22.0 ],
					"style" : "",
					"text" : "cycle~ 0.5"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-79",
					"maxclass" : "newobj",
					"numinlets" : 6,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 451.75, 354.0, 86.0, 22.0 ],
					"style" : "",
					"text" : "scale -1. 1 0 1"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"ft1" : 5.0,
					"id" : "obj-80",
					"maxclass" : "number~",
					"mode" : 2,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 459.5, 327.0, 56.0, 22.0 ],
					"sig" : 0.0,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-81",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 459.5, 303.0, 55.0, 22.0 ],
					"style" : "",
					"text" : "cycle~ 2"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-75",
					"maxclass" : "newobj",
					"numinlets" : 6,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 391.75, 488.0, 90.0, 22.0 ],
					"style" : "",
					"text" : "scale -1. 1 0 -1"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"ft1" : 5.0,
					"id" : "obj-73",
					"maxclass" : "number~",
					"mode" : 2,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 399.5, 461.0, 56.0, 22.0 ],
					"sig" : 0.0,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-74",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 399.5, 437.0, 98.0, 22.0 ],
					"style" : "",
					"text" : "cycle~ 0.015625"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-71",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "float" ],
					"patching_rect" : [ 318.0, 485.0, 35.0, 22.0 ],
					"style" : "",
					"text" : "* 0.5"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"ft1" : 5.0,
					"id" : "obj-69",
					"maxclass" : "number~",
					"mode" : 2,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 281.0, 461.0, 56.0, 22.0 ],
					"sig" : 0.0,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-70",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 281.0, 437.0, 92.0, 22.0 ],
					"style" : "",
					"text" : "cycle~ 0.03125"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"ft1" : 5.0,
					"id" : "obj-63",
					"maxclass" : "number~",
					"mode" : 2,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 281.0, 410.0, 56.0, 22.0 ],
					"sig" : 0.0,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-64",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 281.0, 386.0, 85.0, 22.0 ],
					"style" : "",
					"text" : "cycle~ 0.0625"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"ft1" : 5.0,
					"id" : "obj-61",
					"maxclass" : "number~",
					"mode" : 2,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 362.5, 410.0, 56.0, 22.0 ],
					"sig" : 0.0,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-62",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 362.5, 386.0, 78.0, 22.0 ],
					"style" : "",
					"text" : "cycle~ 0.125"
				}

			}
, 			{
				"box" : 				{
					"fontface" : 0,
					"fontname" : "Arial",
					"fontsize" : 12.0,
					"ft1" : 5.0,
					"id" : "obj-60",
					"maxclass" : "number~",
					"mode" : 2,
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "float" ],
					"patching_rect" : [ 435.0, 410.0, 56.0, 22.0 ],
					"sig" : 0.0,
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-59",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "signal" ],
					"patching_rect" : [ 435.0, 386.0, 55.0, 22.0 ],
					"style" : "",
					"text" : "cycle~ 1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-57",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 54.5, 145.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-58",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 54.5, 200.0, 82.0, 22.0 ],
					"style" : "",
					"text" : "blur_scale $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-55",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 238.0, 15.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-51",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 314.5, 53.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-45",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 107.0, 53.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-47",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 107.0, 81.0, 84.0, 22.0 ],
					"style" : "",
					"text" : "warp_grad $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-33",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 158.5, 145.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-37",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 158.5, 200.0, 53.0, 22.0 ],
					"style" : "",
					"text" : "drive $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-38",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 366.5, 53.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-39",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 366.5, 105.0, 90.0, 22.0 ],
					"style" : "",
					"text" : "agent_drive $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-43",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 314.5, 81.0, 90.0, 22.0 ],
					"style" : "",
					"text" : "warp_agent $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-27",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 262.5, 53.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-28",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 262.5, 107.0, 53.0, 22.0 ],
					"style" : "",
					"text" : "ydrift $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-29",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 210.5, 53.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-32",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 210.5, 81.0, 53.0, 22.0 ],
					"style" : "",
					"text" : "xdrift $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-18",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 159.0, 53.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-26",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 159.0, 107.0, 57.0, 22.0 ],
					"style" : "",
					"text" : "zoom $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-13",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 54.5, 53.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-17",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 54.5, 107.0, 86.0, 22.0 ],
					"style" : "",
					"text" : "warp_color $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-5",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 106.5, 145.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-10",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 106.5, 173.0, 81.0, 22.0 ],
					"style" : "",
					"text" : "blur_initial $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-24",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 387.5, 236.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-25",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 387.5, 264.0, 77.0, 22.0 ],
					"style" : "",
					"text" : "path_blur $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-20",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 305.0, 236.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-21",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 305.0, 264.0, 80.0, 22.0 ],
					"style" : "",
					"text" : "fade_time $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-22",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 220.0, 236.0, 80.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-23",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 220.0, 264.0, 80.0, 22.0 ],
					"style" : "",
					"text" : "path_jitter $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-11",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 98.0, 236.0, 85.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-19",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 98.0, 264.0, 117.0, 22.0 ],
					"style" : "",
					"text" : "momentum_time $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-1",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 3.0, 236.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-3",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 3.0, 264.0, 85.0, 22.0 ],
					"style" : "",
					"text" : "agent_rate $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-14",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 314.5, 145.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-15",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 314.5, 173.0, 83.0, 22.0 ],
					"style" : "",
					"text" : "target_mix $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-12",
					"maxclass" : "button",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "bang" ],
					"patching_rect" : [ 150.5, 383.0, 24.0, 24.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-7",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "int" ],
					"patching_rect" : [ 150.5, 414.0, 95.0, 22.0 ],
					"style" : "",
					"text" : "random 100000"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-6",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 150.5, 476.0, 53.0, 22.0 ],
					"style" : "",
					"text" : "seed $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-2",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 150.5, 445.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-56",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 262.5, 145.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-54",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 3.0, 53.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-52",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 418.0, 145.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-50",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 210.0, 145.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-48",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 366.0, 145.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-46",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 366.0, 200.0, 41.0, 22.0 ],
					"style" : "",
					"text" : "rot $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-42",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 210.0, 173.0, 81.0, 22.0 ],
					"style" : "",
					"text" : "target_sat $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-44",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 262.5, 200.0, 95.0, 22.0 ],
					"style" : "",
					"text" : "target_mean $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-40",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 418.0, 173.0, 85.0, 22.0 ],
					"style" : "",
					"text" : "bound_clip $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-36",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 3.0, 461.0, 41.0, 22.0 ],
					"style" : "",
					"text" : "zl.join"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-16",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"patching_rect" : [ 3.0, 411.0, 57.0, 22.0 ],
					"style" : "",
					"text" : "zl.slice 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-9",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 3.0, 436.0, 110.0, 22.0 ],
					"style" : "",
					"text" : "sprintf /params/%s"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-34",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 470.0, 200.0, 85.0, 22.0 ],
					"style" : "",
					"text" : "time_scale $1"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-35",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 470.0, 145.0, 50.0, 22.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-4",
					"maxclass" : "ezadc~",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "signal", "signal" ],
					"patching_rect" : [ 3.0, 6.0, 31.0, 31.0 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-31",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 3.0, 81.0, 105.0, 22.0 ],
					"style" : "",
					"text" : "disp_exponent $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-30",
					"maxclass" : "preset",
					"numinlets" : 1,
					"numoutlets" : 4,
					"outlettype" : [ "preset", "int", "preset", "int" ],
					"patching_rect" : [ 40.5, 6.0, 100.0, 40.0 ],
					"preset_data" : [ 						{
							"number" : 1,
							"data" : [ 5, "obj-35", "flonum", "float", 0.5, 5, "obj-48", "flonum", "float", 0.0, 5, "obj-50", "flonum", "float", 1.0, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", -0.3, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-2", "number", "int", 42924, 5, "obj-14", "flonum", "float", 0.5, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.000001, 5, "obj-20", "flonum", "float", 4.0, 5, "obj-24", "flonum", "float", 1.0, 5, "obj-5", "flonum", "float", 4.0, 5, "obj-13", "flonum", "float", 2.0, 5, "obj-18", "flonum", "float", 0.0, 5, "obj-29", "flonum", "float", 0.008, 5, "obj-27", "flonum", "float", 0.15, 5, "obj-38", "flonum", "float", 4.0, 5, "obj-33", "flonum", "float", 1.0, 5, "obj-45", "flonum", "float", 2.0, 5, "obj-51", "flonum", "float", 0.0, 5, "obj-57", "flonum", "float", 2.0, 5, "obj-60", "number~", "list", 0.0, 5, "obj-61", "number~", "list", 0.0, 5, "obj-63", "number~", "list", 0.0, 5, "obj-69", "number~", "list", 0.0, 5, "obj-73", "number~", "list", 0.0, 5, "obj-80", "number~", "list", 0.0, 5, "obj-83", "number~", "list", 0.0, 5, "obj-94", "flonum", "float", 0.1 ]
						}
, 						{
							"number" : 2,
							"data" : [ 5, "obj-35", "flonum", "float", 0.5, 5, "obj-48", "flonum", "float", 0.7, 5, "obj-50", "flonum", "float", 0.817, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", -0.5, 5, "obj-56", "flonum", "float", -0.295, 5, "obj-2", "number", "int", 42924, 5, "obj-14", "flonum", "float", 0.9, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.000001, 5, "obj-20", "flonum", "float", 4.0, 5, "obj-24", "flonum", "float", 8.0, 5, "obj-5", "flonum", "float", 3.0, 5, "obj-13", "flonum", "float", 0.0, 5, "obj-18", "flonum", "float", -0.3, 5, "obj-29", "flonum", "float", 0.0, 5, "obj-27", "flonum", "float", 0.0, 5, "obj-38", "flonum", "float", 4.0, 5, "obj-33", "flonum", "float", 1.0, 5, "obj-45", "flonum", "float", 0.0, 5, "obj-51", "flonum", "float", 2.0, 5, "obj-57", "flonum", "float", 1.0, 5, "obj-60", "number~", "list", 0.0, 5, "obj-61", "number~", "list", 0.0, 5, "obj-63", "number~", "list", 0.0, 5, "obj-69", "number~", "list", 0.0, 5, "obj-73", "number~", "list", 0.0, 5, "obj-80", "number~", "list", 0.0, 5, "obj-83", "number~", "list", 0.0, 5, "obj-94", "flonum", "float", 0.0 ]
						}
, 						{
							"number" : 3,
							"data" : [ 5, "obj-35", "flonum", "float", 0.5, 5, "obj-48", "flonum", "float", 1.0, 5, "obj-50", "flonum", "float", 0.9, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", -1.0, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-2", "number", "int", 42924, 5, "obj-14", "flonum", "float", 1.0, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.000001, 5, "obj-20", "flonum", "float", 4.0, 5, "obj-24", "flonum", "float", 8.0, 5, "obj-5", "flonum", "float", 4.0, 5, "obj-13", "flonum", "float", 3.0, 5, "obj-18", "flonum", "float", 0.0, 5, "obj-29", "flonum", "float", 0.0, 5, "obj-27", "flonum", "float", 1.0, 5, "obj-38", "flonum", "float", 3.0, 5, "obj-33", "flonum", "float", 1.0, 5, "obj-45", "flonum", "float", 1.0, 5, "obj-51", "flonum", "float", -0.5, 5, "obj-57", "flonum", "float", 1.0, 5, "obj-60", "number~", "list", 0.0, 5, "obj-61", "number~", "list", 0.0, 5, "obj-63", "number~", "list", 0.0, 5, "obj-69", "number~", "list", 0.0, 5, "obj-73", "number~", "list", 0.0, 5, "obj-80", "number~", "list", 0.0, 5, "obj-83", "number~", "list", 0.0, 5, "obj-94", "flonum", "float", 0.3 ]
						}
, 						{
							"number" : 4,
							"data" : [ 5, "obj-35", "flonum", "float", 1.0, 5, "obj-48", "flonum", "float", 0.0, 5, "obj-50", "flonum", "float", 0.817, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", -0.5, 5, "obj-56", "flonum", "float", -0.295, 5, "obj-2", "number", "int", 42924, 5, "obj-14", "flonum", "float", 0.9, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.000001, 5, "obj-20", "flonum", "float", 0.2, 5, "obj-24", "flonum", "float", 16.0, 5, "obj-5", "flonum", "float", 4.0, 5, "obj-13", "flonum", "float", 32.0, 5, "obj-18", "flonum", "float", 0.5, 5, "obj-29", "flonum", "float", 0.0, 5, "obj-27", "flonum", "float", 0.0, 5, "obj-38", "flonum", "float", 4.0, 5, "obj-33", "flonum", "float", 1.0, 5, "obj-45", "flonum", "float", 1.0, 5, "obj-51", "flonum", "float", 4.0, 5, "obj-57", "flonum", "float", 1.0 ]
						}
, 						{
							"number" : 5,
							"data" : [ 5, "obj-35", "flonum", "float", 0.5, 5, "obj-48", "flonum", "float", 1.0, 5, "obj-50", "flonum", "float", 0.9, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", -1.0, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-2", "number", "int", 42924, 5, "obj-14", "flonum", "float", 1.0, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.000001, 5, "obj-20", "flonum", "float", 10.0, 5, "obj-24", "flonum", "float", 2.0, 5, "obj-5", "flonum", "float", 6.0, 5, "obj-13", "flonum", "float", -33.5, 5, "obj-18", "flonum", "float", 17.0, 5, "obj-29", "flonum", "float", 0.0, 5, "obj-27", "flonum", "float", -10.0, 5, "obj-38", "flonum", "float", 1.0, 5, "obj-33", "flonum", "float", 1.0, 5, "obj-45", "flonum", "float", -375.0, 5, "obj-51", "flonum", "float", -70.0, 5, "obj-57", "flonum", "float", 1.0 ]
						}
, 						{
							"number" : 6,
							"data" : [ 5, "obj-35", "flonum", "float", 1.0, 5, "obj-48", "flonum", "float", 0.0, 5, "obj-50", "flonum", "float", 0.8, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", -0.3, 5, "obj-56", "flonum", "float", -0.3, 5, "obj-2", "number", "int", 88105, 5, "obj-14", "flonum", "float", 0.5, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.000001, 5, "obj-20", "flonum", "float", 4.0, 5, "obj-24", "flonum", "float", 4.0, 5, "obj-5", "flonum", "float", 4.0, 5, "obj-13", "flonum", "float", 16.0, 5, "obj-18", "flonum", "float", 0.57, 5, "obj-29", "flonum", "float", 0.0, 5, "obj-27", "flonum", "float", 0.5, 5, "obj-38", "flonum", "float", 4.0, 5, "obj-33", "flonum", "float", 1.0, 5, "obj-45", "flonum", "float", 8.0, 5, "obj-51", "flonum", "float", 4.0, 5, "obj-57", "flonum", "float", 2.0, 5, "obj-60", "number~", "list", 0.0, 5, "obj-61", "number~", "list", 0.0, 5, "obj-63", "number~", "list", 0.0, 5, "obj-69", "number~", "list", 0.0, 5, "obj-73", "number~", "list", 0.0, 5, "obj-80", "number~", "list", 0.0, 5, "obj-83", "number~", "list", 0.0 ]
						}
, 						{
							"number" : 7,
							"data" : [ 5, "obj-35", "flonum", "float", 0.5, 5, "obj-48", "flonum", "float", 0.7, 5, "obj-50", "flonum", "float", 0.817, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", -0.5, 5, "obj-56", "flonum", "float", -0.295, 5, "obj-2", "number", "int", 5201, 5, "obj-14", "flonum", "float", 0.9, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.000001, 5, "obj-20", "flonum", "float", 4.0, 5, "obj-24", "flonum", "float", 8.0, 5, "obj-5", "flonum", "float", 4.0, 5, "obj-13", "flonum", "float", 32.0, 5, "obj-18", "flonum", "float", -0.3, 5, "obj-29", "flonum", "float", 0.0, 5, "obj-27", "flonum", "float", 0.0, 5, "obj-38", "flonum", "float", 4.0, 5, "obj-33", "flonum", "float", 1.0, 5, "obj-45", "flonum", "float", -32.0, 5, "obj-51", "flonum", "float", 2.0, 5, "obj-57", "flonum", "float", 1.0, 5, "obj-60", "number~", "list", 0.0, 5, "obj-61", "number~", "list", 0.0, 5, "obj-63", "number~", "list", 0.0, 5, "obj-69", "number~", "list", 0.0, 5, "obj-73", "number~", "list", 0.0, 5, "obj-80", "number~", "list", 0.0, 5, "obj-83", "number~", "list", 0.0, 5, "obj-94", "flonum", "float", 0.333 ]
						}
, 						{
							"number" : 8,
							"data" : [ 5, "obj-35", "flonum", "float", 1.0, 5, "obj-48", "flonum", "float", 1.0, 5, "obj-50", "flonum", "float", 0.9, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", -1.0, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-2", "number", "int", 42924, 5, "obj-14", "flonum", "float", 1.0, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.000001, 5, "obj-20", "flonum", "float", 4.0, 5, "obj-24", "flonum", "float", 8.0, 5, "obj-5", "flonum", "float", 6.0, 5, "obj-13", "flonum", "float", -33.5, 5, "obj-18", "flonum", "float", 17.0, 5, "obj-29", "flonum", "float", 0.0, 5, "obj-27", "flonum", "float", -10.0, 5, "obj-38", "flonum", "float", 1.0, 5, "obj-33", "flonum", "float", 1.0, 5, "obj-45", "flonum", "float", -375.0, 5, "obj-51", "flonum", "float", 0.0, 5, "obj-57", "flonum", "float", 1.0 ]
						}
, 						{
							"number" : 9,
							"data" : [ 5, "<invalid>", "flonum", "float", 0.0, 5, "<invalid>", "flonum", "float", 0.99, 5, "<invalid>", "flonum", "float", 0.0, 5, "<invalid>", "flonum", "float", 0.0, 5, "<invalid>", "flonum", "float", 0.0, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-54", "number", "int", 0 ]
						}
, 						{
							"number" : 10,
							"data" : [ 5, "<invalid>", "multislider", "list", 0.2676, 5, "obj-35", "flonum", "float", 0.5, 5, "obj-48", "flonum", "float", 0.666, 5, "obj-50", "flonum", "float", 0.5, 5, "obj-52", "flonum", "float", 2.0, 5, "obj-54", "flonum", "float", 3.0, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-2", "number", "int", 50431 ]
						}
, 						{
							"number" : 11,
							"data" : [ 5, "obj-35", "flonum", "float", 0.5, 5, "obj-48", "flonum", "float", 1.0, 5, "obj-50", "flonum", "float", 0.9, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", -0.5, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-2", "number", "int", 42924, 5, "obj-14", "flonum", "float", 0.5, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.000001, 5, "obj-20", "flonum", "float", 1.0, 5, "obj-24", "flonum", "float", 8.0, 5, "obj-5", "flonum", "float", 4.0, 5, "obj-13", "flonum", "float", 3.0, 5, "obj-18", "flonum", "float", 0.0, 5, "obj-29", "flonum", "float", 0.0, 5, "obj-27", "flonum", "float", 3.0, 5, "obj-38", "flonum", "float", 3.0, 5, "obj-33", "flonum", "float", 1.0, 5, "obj-45", "flonum", "float", 1.0, 5, "obj-51", "flonum", "float", -2.09, 5, "obj-57", "flonum", "float", 1.0 ]
						}
, 						{
							"number" : 12,
							"data" : [ 5, "obj-35", "flonum", "float", 0.175, 5, "obj-48", "flonum", "float", 0.782, 5, "obj-50", "flonum", "float", 0.142, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", 3.0, 5, "obj-56", "flonum", "float", 0.5, 5, "obj-2", "number", "int", 8588, 5, "obj-14", "flonum", "float", 0.67, 5, "obj-1", "flonum", "float", 22.0, 5, "obj-11", "flonum", "float", 0.000112, 5, "obj-22", "flonum", "float", 0.0, 5, "obj-20", "flonum", "float", 0.5, 5, "obj-24", "flonum", "float", 3.0 ]
						}
, 						{
							"number" : 13,
							"data" : [ 5, "obj-35", "flonum", "float", 0.25, 5, "obj-48", "flonum", "float", 0.639, 5, "obj-50", "flonum", "float", 0.5, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", 3.0, 5, "obj-56", "flonum", "float", 0.5, 5, "obj-2", "number", "int", 85118, 5, "obj-14", "flonum", "float", 0.729, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.00001, 5, "obj-20", "flonum", "float", 1.33, 5, "obj-24", "flonum", "float", 3.0 ]
						}
, 						{
							"number" : 14,
							"data" : [ 5, "obj-35", "flonum", "float", 0.25, 5, "obj-48", "flonum", "float", 0.639, 5, "obj-50", "flonum", "float", 0.5, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", 3.0, 5, "obj-56", "flonum", "float", 0.5, 5, "obj-2", "number", "int", 27905, 5, "obj-14", "flonum", "float", 0.729, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.00001, 5, "obj-20", "flonum", "float", 1.33, 5, "obj-24", "flonum", "float", 3.0 ]
						}
, 						{
							"number" : 15,
							"data" : [ 5, "obj-35", "flonum", "float", 0.25, 5, "obj-48", "flonum", "float", 0.71, 5, "obj-50", "flonum", "float", 0.5, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", 3.0, 5, "obj-56", "flonum", "float", 0.5, 5, "obj-2", "number", "int", 27905, 5, "obj-14", "flonum", "float", 0.729, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.000012, 5, "obj-20", "flonum", "float", 7.85, 5, "obj-24", "flonum", "float", 3.0 ]
						}
, 						{
							"number" : 16,
							"data" : [ 5, "obj-35", "flonum", "float", 0.2, 5, "obj-48", "flonum", "float", 0.8, 5, "obj-50", "flonum", "float", 0.2, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", 10.0, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-2", "number", "int", 70793, 5, "obj-14", "flonum", "float", 0.5, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.000053, 5, "obj-22", "flonum", "float", 0.000012, 5, "obj-20", "flonum", "float", 30.0, 5, "obj-24", "flonum", "float", 1.5 ]
						}
, 						{
							"number" : 17,
							"data" : [ 5, "obj-35", "flonum", "float", 0.5, 5, "obj-48", "flonum", "float", 0.5, 5, "obj-50", "flonum", "float", 1.0, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", -0.3, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-2", "number", "int", 42924, 5, "obj-14", "flonum", "float", 1.0, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.000001, 5, "obj-20", "flonum", "float", 4.0, 5, "obj-24", "flonum", "float", 1.0, 5, "obj-5", "flonum", "float", 2.0, 5, "obj-13", "flonum", "float", 2.0, 5, "obj-18", "flonum", "float", 0.5, 5, "obj-29", "flonum", "float", 0.1, 5, "obj-27", "flonum", "float", 0.2, 5, "obj-38", "flonum", "float", 4.0, 5, "obj-33", "flonum", "float", 1.0, 5, "obj-45", "flonum", "float", -1.0, 5, "obj-51", "flonum", "float", 1.0, 5, "obj-57", "flonum", "float", 2.0, 5, "obj-60", "number~", "list", 0.0, 5, "obj-61", "number~", "list", 0.0, 5, "obj-63", "number~", "list", 0.0, 5, "obj-69", "number~", "list", 0.0, 5, "obj-73", "number~", "list", 0.0, 5, "obj-80", "number~", "list", 0.0, 5, "obj-83", "number~", "list", 0.0 ]
						}
, 						{
							"number" : 18,
							"data" : [ 5, "obj-35", "flonum", "float", 1.0, 5, "obj-48", "flonum", "float", 0.0, 5, "obj-50", "flonum", "float", 1.0, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", -0.3, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-2", "number", "int", 42924, 5, "obj-14", "flonum", "float", 1.0, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.000001, 5, "obj-20", "flonum", "float", 4.0, 5, "obj-24", "flonum", "float", 1.0, 5, "obj-5", "flonum", "float", 3.0, 5, "obj-13", "flonum", "float", 2.0, 5, "obj-18", "flonum", "float", 0.0, 5, "obj-29", "flonum", "float", 0.0, 5, "obj-27", "flonum", "float", 0.0, 5, "obj-38", "flonum", "float", 4.0, 5, "obj-33", "flonum", "float", 1.0, 5, "obj-45", "flonum", "float", -2.0, 5, "obj-51", "flonum", "float", 0.0, 5, "obj-57", "flonum", "float", 2.0, 5, "obj-60", "number~", "list", 0.0, 5, "obj-61", "number~", "list", 0.0, 5, "obj-63", "number~", "list", 0.0, 5, "obj-69", "number~", "list", 0.0, 5, "obj-73", "number~", "list", 0.0, 5, "obj-80", "number~", "list", 0.0, 5, "obj-83", "number~", "list", 0.0 ]
						}
, 						{
							"number" : 19,
							"data" : [ 5, "obj-35", "flonum", "float", 0.5, 5, "obj-48", "flonum", "float", 1.0, 5, "obj-50", "flonum", "float", 1.0, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", -1.0, 5, "obj-56", "flonum", "float", -1.0, 5, "obj-2", "number", "int", 42924, 5, "obj-14", "flonum", "float", 0.5, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.000001, 5, "obj-20", "flonum", "float", 4.0, 5, "obj-24", "flonum", "float", 2.0, 5, "obj-5", "flonum", "float", 4.0, 5, "obj-13", "flonum", "float", 3.0, 5, "obj-18", "flonum", "float", 0.0, 5, "obj-29", "flonum", "float", 0.0, 5, "obj-27", "flonum", "float", 1.0, 5, "obj-38", "flonum", "float", 3.0, 5, "obj-33", "flonum", "float", 1.0, 5, "obj-45", "flonum", "float", 1.0, 5, "obj-51", "flonum", "float", 1.0, 5, "obj-57", "flonum", "float", 1.0, 5, "obj-60", "number~", "list", 0.0, 5, "obj-61", "number~", "list", 0.0, 5, "obj-63", "number~", "list", 0.0, 5, "obj-69", "number~", "list", 0.0, 5, "obj-73", "number~", "list", 0.0, 5, "obj-80", "number~", "list", 0.0, 5, "obj-83", "number~", "list", 0.0, 5, "obj-94", "flonum", "float", 0.3 ]
						}
, 						{
							"number" : 20,
							"data" : [ 5, "obj-35", "flonum", "float", 0.5, 5, "obj-48", "flonum", "float", 1.0, 5, "obj-50", "flonum", "float", 0.9, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", -1.0, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-2", "number", "int", 57937, 5, "obj-14", "flonum", "float", 1.0, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.000001, 5, "obj-20", "flonum", "float", 4.0, 5, "obj-24", "flonum", "float", 8.0, 5, "obj-5", "flonum", "float", 4.0, 5, "obj-13", "flonum", "float", 4.0, 5, "obj-18", "flonum", "float", 0.0, 5, "obj-29", "flonum", "float", 0.0, 5, "obj-27", "flonum", "float", 1.0, 5, "obj-38", "flonum", "float", 3.0, 5, "obj-33", "flonum", "float", 1.0, 5, "obj-45", "flonum", "float", 4.0, 5, "obj-51", "flonum", "float", -0.5, 5, "obj-57", "flonum", "float", 1.0, 5, "obj-60", "number~", "list", 0.0, 5, "obj-61", "number~", "list", 0.0, 5, "obj-63", "number~", "list", 0.0, 5, "obj-69", "number~", "list", 0.0, 5, "obj-73", "number~", "list", 0.0, 5, "obj-80", "number~", "list", 0.0, 5, "obj-83", "number~", "list", 0.0, 5, "obj-94", "flonum", "float", 0.1 ]
						}
, 						{
							"number" : 24,
							"data" : [ 5, "obj-35", "flonum", "float", 0.5, 5, "obj-48", "flonum", "float", 0.9, 5, "obj-50", "flonum", "float", 1.0, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", 10.0, 5, "obj-56", "flonum", "float", -0.2, 5, "obj-2", "number", "int", 70793, 5, "obj-14", "flonum", "float", 0.5, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.000053, 5, "obj-22", "flonum", "float", 0.000012, 5, "obj-20", "flonum", "float", 30.0, 5, "obj-24", "flonum", "float", 1.5, 5, "obj-5", "flonum", "float", 1.0, 5, "obj-13", "flonum", "float", 1.0, 5, "obj-18", "flonum", "float", 1.0 ]
						}
 ],
					"style" : ""
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 3.0, 488.0, 137.0, 22.0 ],
					"style" : "",
					"text" : "udpsend localhost 6666"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-3", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-1", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-10", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-19", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-11", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-7", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-12", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-17", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-13", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-15", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-14", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-15", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-36", 1 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-16", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-9", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-16", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-17", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-26", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-18", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-19", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-6", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-2", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-21", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-20", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-21", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-23", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-22", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-23", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-25", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-24", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-25", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-26", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-28", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-27", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-28", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-32", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-29", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-3", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-55", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-30", 2 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-31", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-32", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-37", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-33", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-34", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-34", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-35", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-8", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-36", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-37", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-39", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-38", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-39", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-40", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-42", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-43", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-44", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-47", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-45", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-46", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-47", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-46", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-48", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-10", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-5", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-42", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-50", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-43", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-51", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-40", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-52", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-31", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-54", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-55", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-45", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-55", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-5", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-55", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-51", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-55", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-44", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-56", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-58", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-57", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-58", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-60", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-59", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-6", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-27", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-60", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-51", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-61", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-61", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-62", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-45", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-63", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-63", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-64", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-71", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-69", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-2", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-7", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-69", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-70", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-71", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-75", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-73", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-73", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-74", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-54", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-75", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-18", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-79", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-79", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-80", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-80", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-81", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-85", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-83", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-83", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-84", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-56", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-85", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-36", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-9", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-95", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-94", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-95", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-97", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-96", 0 ]
				}

			}
 ],
		"dependency_cache" : [  ],
		"embedsnapshot" : 0
	}

}
