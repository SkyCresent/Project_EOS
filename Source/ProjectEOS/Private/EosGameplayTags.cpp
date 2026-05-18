// Fill out your copyright notice in the Description page of Project Settings.


#include "EosGameplayTags.h"

namespace EosGameplayTags
{
	/** Input Tags **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Interact, "InputTag.Interact");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Inventory, "InputTag.Inventory");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Dash, "InputTag.Dash");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Guard, "InputTag.Guard");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipWeapon, "InputTag.EquipWeapon");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnequipWeapon, "InputTag.UnequipWeapon");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Guard_Hold, "InputTag.Guard.Hold");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle, "InputTag.Toggle");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Toggle_TargetLock, "InputTag.Toggle.TargetLock");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SwitchTarget, "InputTag.SwitchTarget");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_FocusOnChat, "InputTag.FocusOnChat");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Continue, "InputTag.Continue");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Continue_Skill_E, "InputTag.Continue.Skill.E");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_Skill_Q, "InputTag.Skill.Q");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Skill_E, "InputTag.Skill.E");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Skill_R, "InputTag.Skill.R");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Skill_F, "InputTag.Skill.F");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_QuickSlot_1, "InputTag.QuickSlot.1");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_QuickSlot_2, "InputTag.QuickSlot.2");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_QuickSlot_3, "InputTag.QuickSlot.3");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_QuickSlot_4, "InputTag.QuickSlot.4");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_GreatSword, "InputTag.LightAttack.GreatSword");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SmashAttack_GreatSword, "InputTag.SmashAttack.GreatSword");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Healer, "InputTag.LightAttack.Healer");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SmashAttack_Healer, "InputTag.SmashAttack.Healer");

	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Lancer, "InputTag.LightAttack.Lancer");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_SmashAttack_Lancer, "InputTag.SmashAttack.Lancer");
	
	/** Weapon Tags : Player **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_GreatSword, "Player.Weapon.GreatSword");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Staff, "Player.Weapon.Staff");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Lance, "Player.Weapon.Lance");

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_EquipWeapon, "Player.Event.EquipWeapon");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_UnequipWeapon, "Player.Event.UnequipWeapon");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_GuardSuccess, "Player.Event.GuardSuccess");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Left, "Player.Event.SwitchTarget.Left");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_SwitchTarget_Right, "Player.Event.SwitchTarget.Right");

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Spawn_Right, "Player.Event.Spawn.Right");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Spawn_Left, "Player.Event.Spawn.Left");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Spawn_TwoHands, "Player.Event.Spawn.TwoHands");

	/** Weapon Tags : Enemy **/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon_Hammer, "Enemy.Weapon.Hammer");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon_Sword, "Enemy.Weapon.Sword");

	/** Player Ability Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_GreatSword, "Player.Ability.Attack.Light.GreatSword");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Smash_GreatSword, "Player.Ability.Attack.Smash.GreatSword");

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_Healer, "Player.Ability.Attack.Light.Healer");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Smash_Healer, "Player.Ability.Attack.Smash.Healer");

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_Lancer, "Player.Ability.Attack.Light.Lancer");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Smash_Lancer, "Player.Ability.Attack.Smash.Lancer");

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Dash, "Player.Ability.Dash");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Guarding, "Player.Ability.Guarding");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_TargetLock, "Player.Ability.TargetLock");

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Skill_Q, "Player.Ability.Skill.Q");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Skill_E, "Player.Ability.Skill.E");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Skill_R, "Player.Ability.Skill.R");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Skill_F, "Player.Ability.Skill.F");

	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Items_Potion, "Player.Ability.Items.Potion");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Items_Artifact, "Player.Ability.Items.Artifact");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_InCombat, "Player.Status.InCombat");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_OpenMenu, "Player.Status.OpenMenu");


	/** Player Status Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Weapon_Equip, "Player.Status.Weapon.Equip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Weapon_Unequip, "Player.Status.Weapon.Unequip");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Light_Combo1, "Player.Status.Light.Combo1");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Light_Combo2, "Player.Status.Light.Combo2");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Light_Combo3, "Player.Status.Light.Combo3");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Light_Combo4, "Player.Status.Light.Combo4");
	//UE_DEFINE_GAMEPLAY_TAG(Player_Status_Light_Combo4, "Player.Status.Light.Combo5");
	//UE_DEFINE_GAMEPLAY_TAG(Player_Status_Light_Combo4, "Player.Status.Light.Combo6");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Smash_InProgress, "Player.Status.Smash.InProgress");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Smash_EndProgress, "Player.Status.Smash.EndProgress");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Smash_Extend, "Player.Status.Smash.Extend");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Counter, "Player.Status.Counter");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Dash, "Player.Status.Dash");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Guarding, "Player.Status.Guarding");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_TargetLock, "Player.Status.TargetLock");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_SuperArmor, "Player.Status.SuperArmor");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Invincibility, "Player.Status.Invincibility");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_GreatSword_StackDash, "Player.Status.GreatSword.StackDash");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_GreatSword_Ironwall, "Player.Status.GreatSword.Ironwall");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_GreatSword_Revengeable, "Player.Status.GreatSword.Revengeable");

	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Items_UsingPotion, "Player.Status.Items.UsingPotion");
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Items_UsingArtifact, "Player.Status.Items.UsingArtifact");

	/** Player Event Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Combo_Start, "Player.Event.Combo.Start");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Combo_End, "Player.Event.Combo.End");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Smash_Extended, "Player.Event.Smash.Extended");

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_React_CameraShake, "Player.Event.React.CameraShake");

	UE_DEFINE_GAMEPLAY_TAG(Player_Event_GreatSword_ActivateIronwall, "Player.Event.GreatSword.ActivateIronwall");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_GreatSword_SuccessIronwall, "Player.Event.GreatSword.SuccessIronwall");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Skill_GreatSword_PowerLevel1, "Player.Event.Skill.GreatSword.PowerLevel1");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Skill_GreatSword_PowerLevel2, "Player.Event.Skill.GreatSword.PowerLevel2");


	/** Player SetByCaller Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Light, "Player.SetByCaller.AttackType.Light");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Smash, "Player.SetByCaller.AttackType.Smash");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Skill, "Player.SetByCaller.AttackType.Skill");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_Status_Stamina, "Player.SetByCaller.Status.Stamina");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_Status_Mana, "Player.SetByCaller.Status.Mana");
	//UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_Status_Identity, "Player.SetByCaller.Status.Identity");

	/** Player Cooldown Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Cooldown_Skill_Q, "Player.Cooldown.Skill.Q");
	UE_DEFINE_GAMEPLAY_TAG(Player_Cooldown_Skill_E, "Player.Cooldown.Skill.E");
	UE_DEFINE_GAMEPLAY_TAG(Player_Cooldown_Skill_R, "Player.Cooldown.Skill.R");
	UE_DEFINE_GAMEPLAY_TAG(Player_Cooldown_Skill_F, "Player.Cooldown.Skill.F");

	/** Enemy Ability Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee, "Enemy.Ability.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Combo, "Enemy.Ability.Combo");

	/** Enemy Boss Ability Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_PrePhase_Melee, "Enemy.Ability.Boss.PrePhase.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_PrePhase_Combo, "Enemy.Ability.Boss.PrePhase.Combo");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_PrePhase_Dash, "Enemy.Ability.Boss.PrePhase.Dash");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase1_Melee, "Enemy.Ability.Boss.Phase1.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase1_Combo, "Enemy.Ability.Boss.Phase1.Combo");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase1_Dash, "Enemy.Ability.Boss.Phase1.Dash");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase1_PhaseSkill, "Enemy.Ability.Boss.Phase1.PhaseSkill");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase1_Skill, "Enemy.Ability.Boss.Phase1.Skill");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase2_Melee, "Enemy.Ability.Boss.Phase2.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase2_Combo, "Enemy.Ability.Boss.Phase2.Combo");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase2_Dash, "Enemy.Ability.Boss.Phase2.Dash");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase2_PhaseSkill, "Enemy.Ability.Boss.Phase2.PhaseSkill");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase2_Skill, "Enemy.Ability.Boss.Phase2.Skill");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase3_Melee, "Enemy.Ability.Boss.Phase3.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase3_Combo, "Enemy.Ability.Boss.Phase3.Combo");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase3_Dash, "Enemy.Ability.Boss.Phase3.Dash");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase3_PhaseSkill, "Enemy.Ability.Boss.Phase3.PhaseSkill");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Boss_Phase3_Skill, "Enemy.Ability.Boss.Phase3.Skill");

	/** Enemy Status Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Strafing, "Enemy.Status.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Stunned, "Enemy.Status.Stunned");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_BreakGuard, "Enemy.Status.BreakGuard");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Counterable, "Enemy.Status.Counterable");

	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_UnderAttack, "Enemy.Status.UnderAttack");

	/** Enemy Boss Status Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Boss, "Enemy.Status.Boss");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Boss_PrePhase, "Enemy.Status.Boss.PrePhase");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Boss_Phase1, "Enemy.Status.Boss.Phase1");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Boss_Phase2, "Enemy.Status.Boss.Phase2");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Boss_Phase3, "Enemy.Status.Boss.Phase3");

	/** Enemy Event Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_SuccessCounter, "Enemy.Event.SuccessCounter");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Summoning, "Enemy.Event.Summoning");

	/** Enemy Boss Event Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Boss_PrePhase, "Enemy.Event.Boss.PrePhase");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Boss_Phase1, "Enemy.Event.Boss.Phase1");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Boss_Phase2, "Enemy.Event.Boss.Phase2");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_Boss_Phase3, "Enemy.Event.Boss.Phase3");

	/** Common Ability Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Common_Ability_Dead, "Common.Ability.Dead");
	UE_DEFINE_GAMEPLAY_TAG(Common_Ability_HitReact, "Common.Ability.HitReact");
	
	/** Common Status Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_HitReact_Front, "Common.Status.HitReact.Front");
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_HitReact_Left, "Common.Status.HitReact.Left");
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_HitReact_RIght, "Common.Status.HitReact.Right");
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_HitReact_Back, "Common.Status.HitReact.Back");
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_Dead, "Common.Status.Dead");

	/** Common Event Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_MeleeHit, "Common.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_RangeHit, "Common.Event.RangeHit");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_SkillSpawn, "Common.Event.SkillSpawn");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_HitReact_Heavy, "Common.Event.HitReact.Heavy");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_HitReact_Light, "Common.Event.HitReact.Light");

	/** Common SetByCaller Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Common_SetByCaller_BaseDamage, "Common.SetByCaller.BaseDamage"); 

	/** GameplayCur Tags **/
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Feedback_HoldAnim, "GameplayCue.Feedback.HoldAnim"); 
}