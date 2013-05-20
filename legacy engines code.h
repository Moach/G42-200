/*
	
	FXMapValue(fxRef, thrFX_map, thrFX_X, thrFX_Y);
	
	
	// red channel is for the turbo-jets (in open mode)
	
	double thrLvl = GetEngineLevel(ENGINE_MAIN);
	
	if (inltDoors->pos < 0.5)  // inlets open!
	{
		
		if (main_eng_mode == 1)
		{
			thrPos += ((thrLvl -  thrPos) * ENGINE_RAMP_SCALAR * dT);


			//
			SetThrusterIsp(RT66_gasGen_thgr[0],  ISPMAX_MAIN_GEN * fxRef.channelRed);
			SetThrusterIsp(RT66_gasGen_thgr[1],  ISPMAX_MAIN_GEN * fxRef.channelRed);
			SetThrusterMax0(RT66_gasGen_thgr[0], MAXTHRUST_MAIN_GEN * fxRef.channelRed);
			SetThrusterMax0(RT66_gasGen_thgr[1], MAXTHRUST_MAIN_GEN * fxRef.channelRed);

			//
			SetThrusterLevel(RT66_gasGen_thgr[0], thrPos);
			SetThrusterLevel(RT66_gasGen_thgr[1], thrPos);

			if (burner_toggle)  // reheat ON! (this is probably a bad idea... a proper variable should be used here)
			{

				SetThrusterIsp(RT66_burner_thgr[0],  ISPMAX_MAIN_AFB * fxRef.channelRed);
				SetThrusterIsp(RT66_burner_thgr[1],  ISPMAX_MAIN_AFB * fxRef.channelRed);
				SetThrusterMax0(RT66_burner_thgr[0], MAXTHRUST_MAIN_AFB * fxRef.channelRed);
				SetThrusterMax0(RT66_burner_thgr[1], MAXTHRUST_MAIN_AFB * fxRef.channelRed);

				double rhtLvl = (max(thrLvl - .8, 0.0) * 5.0) * (max(thrPos - .8, 0.0) * 5.0);
				SetThrusterLevel(RT66_burner_thgr[0], rhtLvl);
				SetThrusterLevel(RT66_burner_thgr[1], rhtLvl);
			} else
			{
				SetThrusterLevel(RT66_burner_thgr[0], 0.0);
				SetThrusterLevel(RT66_burner_thgr[1], 0.0);

			}
		} else
		{
			// gotta remove all thrust when engines are off... otherwise, they don't really stop when we tell them to...
			SetThrusterLevel(RT66_gasGen_thgr[0], 0.0);
			SetThrusterLevel(RT66_gasGen_thgr[1], 0.0);
			SetThrusterLevel(RT66_burner_thgr[0], 0.0);
			SetThrusterLevel(RT66_burner_thgr[1], 0.0);
		}

	} else 
	{

		SetThrusterLevel(RT66_gasGen_thgr[0], 0.0);
		SetThrusterLevel(RT66_gasGen_thgr[1], 0.0);
		SetThrusterLevel(RT66_burner_thgr[0], 0.0);
		SetThrusterLevel(RT66_burner_thgr[1], 0.0);


		if (thr_authority) // OMS throttle mode - reroutes command authority from main engines
		{
			SetThrusterLevel(RT66_rocket_thgr[0], 0.0); // shut them down
			SetThrusterLevel(RT66_rocket_thgr[1], 0.0); //
			//

			//
			SetThrusterLevel(oms_thgr[0], thrLvl);
			SetThrusterLevel(oms_thgr[1], thrLvl);
			
		}
		else if (main_eng_mode == 2)
		{
			SetThrusterLevel(oms_thgr[0], 0.0);
			SetThrusterLevel(oms_thgr[1], 0.0);
			
			thrPos = thrLvl;
			SetThrusterLevel(RT66_rocket_thgr[0], thrLvl);
			SetThrusterLevel(RT66_rocket_thgr[1], thrLvl);
			//

			double flwRate = GetPropellantFlowrate(fuel_main_allTanks);
			double oxyMass = GetPropellantMass(fuel_oxy);
			//
			SetPropellantMass(fuel_oxy, max(0.0, oxyMass - ( flwRate * dT * OXYFUEL_RATIO )));
		}

	}

	

	
	if (ramxDoors->pos > 0.5)  // ramcaster open!
	{
		switch(ramcaster_mode)
		{
		case 1:

			SetThrusterIsp(ramcaster,       ISPMAX_RAMXLO * fxRef.channelGreen);
			SetThrusterMax0(ramcaster,  MAXTHRUST_RAMX_LO * fxRef.channelGreen);
			break;
		case 2:
			SetThrusterIsp(ramcaster,       ISPMAX_RAMXHI * fxRef.channelBlue);
			SetThrusterMax0(ramcaster,  MAXTHRUST_RAMX_HI * fxRef.channelBlue);
			break;
		default:
		case 0:
			//	SetThrusterIsp(ramcaster,      ISPMAX_RAMXHI * fxRef.channelBlue);
			SetThrusterMax0(ramcaster,  0.0);
			break;
		}	
		//
		//SetThrusterLevel(ramcaster, 0.0);
		SetAnimation(an_dvc_ramx, GetThrusterGroupLevel(THGROUP_HOVER));

	} else
	{
		//SetThrusterLevel(ramcaster, 0.0);
		SetThrusterMax0(ramcaster, 0.0);
	}
	
	
	
	*/