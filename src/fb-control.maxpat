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
		"rect" : [ 879.0, 161.0, 399.0, 516.0 ],
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
					"id" : "obj-18",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 184.5, 53.0, 50.0, 22.0 ],
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
					"patching_rect" : [ 184.5, 81.0, 57.0, 22.0 ],
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
					"patching_rect" : [ 120.5, 53.0, 50.0, 22.0 ],
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
					"patching_rect" : [ 120.5, 81.0, 53.0, 22.0 ],
					"style" : "",
					"text" : "warp $1"
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
					"patching_rect" : [ 11.0, 292.0, 50.0, 22.0 ],
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
					"patching_rect" : [ 11.0, 320.0, 75.0, 22.0 ],
					"style" : "",
					"text" : "blur_size $1"
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
					"patching_rect" : [ 267.5, 301.0, 50.0, 22.0 ],
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
					"patching_rect" : [ 267.5, 329.0, 77.0, 22.0 ],
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
					"patching_rect" : [ 265.0, 243.0, 50.0, 22.0 ],
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
					"patching_rect" : [ 265.0, 271.0, 80.0, 22.0 ],
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
					"patching_rect" : [ 265.0, 186.0, 63.0, 22.0 ],
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
					"patching_rect" : [ 265.0, 214.0, 80.0, 22.0 ],
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
					"patching_rect" : [ 265.0, 128.0, 85.0, 22.0 ],
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
					"patching_rect" : [ 265.0, 156.0, 117.0, 22.0 ],
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
					"patching_rect" : [ 265.0, 71.0, 50.0, 22.0 ],
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
					"patching_rect" : [ 265.0, 99.0, 85.0, 22.0 ],
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
					"patching_rect" : [ 120.5, 226.0, 50.0, 22.0 ],
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
					"patching_rect" : [ 120.5, 254.0, 83.0, 22.0 ],
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
					"patching_rect" : [ 151.5, 349.0, 24.0, 24.0 ],
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
					"patching_rect" : [ 151.5, 383.0, 95.0, 22.0 ],
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
					"patching_rect" : [ 151.5, 445.0, 53.0, 22.0 ],
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
					"patching_rect" : [ 151.5, 414.0, 50.0, 22.0 ],
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
					"patching_rect" : [ 120.5, 167.0, 50.0, 22.0 ],
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
					"patching_rect" : [ 11.0, 230.0, 50.0, 22.0 ],
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
					"patching_rect" : [ 11.0, 173.0, 50.0, 22.0 ],
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
					"patching_rect" : [ 120.5, 110.0, 50.0, 22.0 ],
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
					"patching_rect" : [ 11.0, 110.0, 50.0, 22.0 ],
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
					"patching_rect" : [ 11.0, 138.0, 41.0, 22.0 ],
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
					"patching_rect" : [ 120.5, 138.0, 81.0, 22.0 ],
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
					"patching_rect" : [ 120.5, 195.0, 95.0, 22.0 ],
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
					"patching_rect" : [ 11.0, 201.0, 85.0, 22.0 ],
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
					"patching_rect" : [ 11.0, 454.0, 41.0, 22.0 ],
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
					"patching_rect" : [ 11.0, 390.0, 57.0, 22.0 ],
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
					"patching_rect" : [ 11.0, 420.0, 110.0, 22.0 ],
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
					"patching_rect" : [ 120.5, 316.0, 85.0, 22.0 ],
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
					"patching_rect" : [ 120.5, 288.0, 50.0, 22.0 ],
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
					"patching_rect" : [ 11.0, 6.0, 45.0, 45.0 ],
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
					"patching_rect" : [ 11.0, 258.0, 95.0, 22.0 ],
					"style" : "",
					"text" : "kernel_width $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-30",
					"maxclass" : "preset",
					"numinlets" : 1,
					"numoutlets" : 4,
					"outlettype" : [ "preset", "int", "preset", "int" ],
					"patching_rect" : [ 65.5, 6.0, 100.0, 40.0 ],
					"preset_data" : [ 						{
							"number" : 1,
							"data" : [ 5, "obj-35", "flonum", "float", 0.25, 5, "obj-48", "flonum", "float", 0.639, 5, "obj-50", "flonum", "float", 0.5, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", 3.0, 5, "obj-56", "flonum", "float", 0.5, 5, "obj-2", "number", "int", 29254, 5, "obj-14", "flonum", "float", 0.729, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.0001, 5, "obj-22", "flonum", "float", 0.00001, 5, "obj-20", "flonum", "float", 1.33, 5, "obj-24", "flonum", "float", 3.0 ]
						}
, 						{
							"number" : 2,
							"data" : [ 5, "obj-35", "flonum", "float", 0.2, 5, "obj-48", "flonum", "float", 0.666, 5, "obj-50", "flonum", "float", 1.0, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", 10.0, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-2", "number", "int", 42924, 5, "obj-14", "flonum", "float", 0.2, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.000053, 5, "obj-22", "flonum", "float", 0.000012, 5, "obj-20", "flonum", "float", 4.0, 5, "obj-24", "flonum", "float", 2.0, 5, "obj-5", "flonum", "float", 16.0, 5, "obj-13", "flonum", "float", 1.0 ]
						}
, 						{
							"number" : 3,
							"data" : [ 5, "obj-35", "flonum", "float", 0.3, 5, "obj-48", "flonum", "float", -0.5, 5, "obj-50", "flonum", "float", 0.7, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", 10.0, 5, "obj-56", "flonum", "float", -0.5, 5, "obj-2", "number", "int", 42924, 5, "obj-14", "flonum", "float", 0.5, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.000053, 5, "obj-22", "flonum", "float", 0.000012, 5, "obj-20", "flonum", "float", 4.0, 5, "obj-24", "flonum", "float", 1.0, 5, "obj-5", "flonum", "float", 2.0, 5, "obj-13", "flonum", "float", 1.0 ]
						}
, 						{
							"number" : 4,
							"data" : [ 5, "obj-35", "flonum", "float", 0.3, 5, "obj-48", "flonum", "float", 0.5, 5, "obj-50", "flonum", "float", 1.0, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", 10.0, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-2", "number", "int", 42924, 5, "obj-14", "flonum", "float", 0.666, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.000053, 5, "obj-22", "flonum", "float", 0.000012, 5, "obj-20", "flonum", "float", 4.0, 5, "obj-24", "flonum", "float", 1.0, 5, "obj-5", "flonum", "float", 8.0, 5, "obj-13", "flonum", "float", 4.0 ]
						}
, 						{
							"number" : 5,
							"data" : [ 5, "obj-35", "flonum", "float", 0.5, 5, "obj-48", "flonum", "float", -0.6666, 5, "obj-50", "flonum", "float", 0.7, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", 10.0, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-2", "number", "int", 67661, 5, "obj-14", "flonum", "float", 1.0, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.000053, 5, "obj-22", "flonum", "float", 0.000012, 5, "obj-20", "flonum", "float", 2.0, 5, "obj-24", "flonum", "float", 1.0, 5, "obj-5", "flonum", "float", 2.0, 5, "obj-13", "flonum", "float", 1.0 ]
						}
, 						{
							"number" : 6,
							"data" : [ 5, "<invalid>", "multislider", "list", 0.2676, 5, "obj-35", "flonum", "float", 0.3, 5, "obj-48", "flonum", "float", 0.5, 5, "obj-50", "flonum", "float", 0.0, 5, "obj-52", "flonum", "float", 2.0, 5, "obj-54", "flonum", "float", 3.0, 5, "obj-56", "flonum", "float", 0.0, 5, "obj-2", "number", "int", 19710 ]
						}
, 						{
							"number" : 7,
							"data" : [ 5, "<invalid>", "flonum", "float", 2.0, 5, "<invalid>", "flonum", "float", 100.0, 5, "<invalid>", "flonum", "float", 0.0, 5, "<invalid>", "flonum", "float", 0.0, 5, "<invalid>", "flonum", "float", 14.303572, 5, "obj-56", "flonum", "float", -0.01, 5, "obj-54", "number", "int", 1, 5, "<invalid>", "flonum", "float", 0.0, 5, "<invalid>", "multislider", "list", 0.476786, 5, "obj-52", "flonum", "float", 16.0 ]
						}
, 						{
							"number" : 8,
							"data" : [ 5, "<invalid>", "flonum", "float", -2.0, 5, "<invalid>", "flonum", "float", 3.0, 5, "<invalid>", "flonum", "float", 0.2, 5, "<invalid>", "flonum", "float", 0.928, 5, "<invalid>", "flonum", "float", 100.0, 5, "obj-56", "flonum", "float", 1.0, 5, "obj-54", "number", "int", 4, 5, "<invalid>", "flonum", "float", 0.0, 5, "<invalid>", "multislider", "list", 0.0, 5, "obj-52", "flonum", "float", 4.0, 5, "obj-35", "flonum", "float", 0.9 ]
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
							"data" : [ 5, "obj-35", "flonum", "float", 0.285, 5, "obj-48", "flonum", "float", 0.782, 5, "obj-50", "flonum", "float", 0.142, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", 3.0, 5, "obj-56", "flonum", "float", 0.5, 5, "obj-2", "number", "int", 14640, 5, "obj-14", "flonum", "float", 0.67, 5, "obj-1", "flonum", "float", 100.0, 5, "obj-11", "flonum", "float", 0.000112, 5, "obj-22", "flonum", "float", 0.0, 5, "obj-20", "flonum", "float", 0.2, 5, "obj-24", "flonum", "float", 3.0 ]
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
							"data" : [ 5, "obj-35", "flonum", "float", 0.1, 5, "obj-48", "flonum", "float", 1.0, 5, "obj-50", "flonum", "float", 0.2, 5, "obj-52", "flonum", "float", 1.0, 5, "obj-54", "flonum", "float", 10.0, 5, "obj-56", "flonum", "float", -0.5, 5, "obj-2", "number", "int", 70793, 5, "obj-14", "flonum", "float", 0.2, 5, "obj-1", "flonum", "float", 33.0, 5, "obj-11", "flonum", "float", 0.000053, 5, "obj-22", "flonum", "float", 0.000012, 5, "obj-20", "flonum", "float", 10.0, 5, "obj-24", "flonum", "float", 2.0, 5, "obj-5", "flonum", "float", 4.0, 5, "obj-13", "flonum", "float", 100.0 ]
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
					"patching_rect" : [ 11.0, 483.0, 137.0, 22.0 ],
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
					"destination" : [ "obj-16", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-3", 0 ]
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
					"source" : [ "obj-44", 0 ]
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
					"destination" : [ "obj-44", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-56", 0 ]
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
					"destination" : [ "obj-2", 0 ],
					"disabled" : 0,
					"hidden" : 0,
					"source" : [ "obj-7", 0 ]
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
 ],
		"dependency_cache" : [  ],
		"embedsnapshot" : 0
	}

}
