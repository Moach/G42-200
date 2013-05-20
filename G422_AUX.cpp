
#include "G422.h"
#include "G422_DVC.h"
//
//










int MovingPart::toggle()
{

	if(mp_status ==  MP_REVERSING || mp_status == MP_LOW_DETENT)
	{
		mp_status = MP_MOVING;
		sysReset  = false;
		vsl->clbkGeneric(VMSG_MPSTRT, sysID, this);
	}
	else if(mp_status == MP_MOVING || mp_status == MP_HI_DETENT)
	{
		mp_status = MP_REVERSING;
		sysReset  = false;
		vsl->clbkGeneric(VMSG_MPSTRT, sysID, this);
	}
	
	return mp_status;
}

int MovingPart::toggle(bool b)
{

	if(b && (mp_status ==  MP_REVERSING || mp_status == MP_LOW_DETENT))
	{
		mp_status = MP_MOVING;
		sysReset  = false;
		vsl->clbkGeneric(VMSG_MPSTRT, sysID, this);
	}
	else if(!b && (mp_status == MP_MOVING || mp_status == MP_HI_DETENT))
	{
		mp_status = MP_REVERSING;
		sysReset  = false;
		vsl->clbkGeneric(VMSG_MPSTRT, sysID, this);
	}

	return mp_status;
}


void MovingPart::toDetent(int p)
{

	if (p == MP_HI_DETENT)
	{
		mp_status = MP_HI_DETENT;
		pos = 1;
		
	} else
	{
		mp_status = MP_LOW_DETENT;
		pos = 0;
		
	}
	vsl->SetAnimation (anim_idx, pos);
	
	sysReset  = false;
	vsl->clbkGeneric(VMSG_MPSTRT, sysID, this);
}


bool MovingPart::getToggleState()
{
	if ((mp_status ==  MP_REVERSING || mp_status == MP_LOW_DETENT))
	{
		return false;
	}
	if((mp_status == MP_MOVING || mp_status == MP_HI_DETENT))
	{
		return true;
	}
	
	return false;
}



//
//
//  not sure where else to put these....


bool VCSwitch::flick(bool upDn, VESSEL3 *vslRef) // true = "up" | false = "down"
{
	//
	switch (pos)
	{
		case SW2_DOWN:
			if (upDn)
			{
				vslRef->SetAnimation(anID, 1.0);
				pos = SW2_UP;
				return true;
			}
		break;
		case SW2_UP:
			if (!upDn)
			{
				vslRef->SetAnimation(anID, 0.0);
				pos = SW2_DOWN;
				return true;
			}
		break;
		case SW3_DOWN:
			if (upDn)
			{
				vslRef->SetAnimation(anID, 0.5);
				pos = SW3_MID;
				return true;
			}
		break;
		case SW3_MID:
			if (upDn)
			{
				vslRef->SetAnimation(anID, 1.0);
				pos = SW3_UP;
				return true;
			} else
			{
				vslRef->SetAnimation(anID, 0.0);
				pos = SW3_DOWN;
				return true;
			}
		break;
		case SW3_UP:
			if (!upDn)
			{
				vslRef->SetAnimation(anID, 0.5);
				pos = SW3_MID;
				return true;
			}
		break;
	}

	return false; // false is returned when the switch cannot be flipped to that position
}


void VCSwitch::setPos(VCSwitchState newPos, VESSEL3 *vslRef)
{
	//
	if (pos == newPos) return;
	
	pos = newPos;
	switch (newPos)
	{
		case SW2_UP:
		case SW3_UP:
			vslRef->SetAnimation(anID, 1.0);
		return;

		case SW3_MID:
			vslRef->SetAnimation(anID, 0.5);
		return;
		case SW2_DOWN:
		case SW3_DOWN:
			vslRef->SetAnimation(anID, 0.0);
		return;
	}
}





