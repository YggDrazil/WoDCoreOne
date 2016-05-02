
/* ScriptData
SDName: boss_spine_of_Deathwing
SD%Complete: 95%
SDComment: Not implemented
SDCategory: Kalimdor, CavernsOfTime, DragonSoul
EndScriptData */

#include "ScriptPCH.h"
#include "dragon_soul.h"
#include "GameObjectAI.h"


#define SPELL_BURST				RAID_MODE<int32>(105219, 109371, 109372, 109373)
#define SPELL_SEARING_PLASMA_2	RAID_MODE<int32>(105479, 109362, 109363, 109364)
#define SPELL_FIERY_GRIP		RAID_MODE<int32>(105490, 109457, 109458, 109459)
#define AURA_GASPING_TENDRILS	RAID_MODE<int32>(105563, 109454, 109455, 109456)

enum Spells
{
	SPELL_SEARING_PLASMA_1 = 109379,
	SPELL_NUCLEAR_BLAST = 105845,
	SPELL_SUPERHEATED_NUCLEUS = 105834,
	SPELL_ABSORBED_BLOOD = 105248,
	SPELL_ABSORBED_BLOOD_BAR = 109329,
	SPELL_GRASPING_TENDRILS = 105510,
	SPELL_DEGRADATION = 106005,

	SPELL_CORRUPTION_DEATH = 106199,
	SPELL_CORRUPTION_EARTH = 106200,
	SPELL_BLOOD_OF_NELTHARION = 106213,
	SPELL_BLOOD_OF_DEATHWING = 106201,

	SPELL_BREACH_ARMOR_LEFT = 105363,
	SPELL_BREACH_ARMOR_RIGHT = 105385,
	SPELL_PLATE_FLY_OFF_LEFT = 105366,
	SPELL_PLATE_FLY_OFF_RIGHT = 105384,
	SPELL_SEAL_ARMOR_BREACH_LEFT = 105847,
	SPELL_SEAL_ARMOR_BREACH_RIGHT = 105848,
};

enum Events
{
	EVENT_AMALGAMATION_EXPLODE = 1,
	EVENT_CORRUPTION_GRIP = 2,
	EVENT_CORRUPTION_PLASMA_1 = 3,
	EVENT_CORRUPTION_PLASMA_2 = 4,
	EVENT_BLOOD_SPAWNER = 5,
	EVENT_SPAWNER_BLOOD = 6,
	EVENT_SPINE_BALANCE_CHECK = 7,
	EVENT_SPINE_ROLL = 8,
};

enum DataActions {
	ACTION_CLEAR_UNITS = 0,
	ACTION_CLEAR_PLATES = 1,
	ACTION_FINISH_HIM = 2,
};

Position const SpawnerPositions[10] = {
	{ -13868.50f, -13667.30f, 261.92f, 0.0f },
	{ -13841.60f, -13667.00f, 262.08f, 3.14f },
	{ -13868.50f, -13654.10f, 262.86f, 0.0f },
	{ -13842.52f, -13654.50f, 263.19f, 3.14f },
	{ -13866.90f, -13638.30f, 264.80f, 0.0f },
	{ -13841.40f, -13635.00f, 265.30f, 3.14f },
	{ -13870.50f, -13614.30f, 266.89f, 0.0f },
	{ -13839.60f, -13614.50f, 266.39f, 3.14f },
	{ -13871.86f, -13596.38f, 269.13f, 0.0f },
	{ -13837.62f, -13596.55f, 268.35f, 3.14f },
};

Position const PlatePosition[3] = {
	{ -13862.80f, -13645.00f, 265.77f, 1.58f },
	{ -13862.60f, -13626.30f, 266.79f, 1.58f },
	{ -13862.60f, -13604.90f, 269.65f, 1.58f },
};

class AmalgamationPlayersCheck
{
public:
	AmalgamationPlayersCheck(WorldObject const* obj) : _obj(obj) { }
	bool operator()(Player* u)
	{
		if (!u->isAlive())
			return false;

		if (!_obj->IsWithinDistInMap(u, 200.0f))
			return false;

		if (u->HasAura(SPELL_CORRUPTION_DEATH) || u->HasAura(SPELL_CORRUPTION_EARTH))
			return false;

		if (_obj->GetGUID() == u->GetGUID())
			return false;

		return true;
	}

private:
	WorldObject const* _obj;
};

class boss_deathwing_spine : public CreatureScript
{
public:
	boss_deathwing_spine() : CreatureScript("boss_deathwing_spine") { }

	struct boss_deathwing_spineAI : public BossAI
	{
		boss_deathwing_spineAI(Creature* creature) : BossAI(creature, NPC_DEATHWING_SPINE) { }

		std::list<Creature*> units;
		GameObject* plate;
		EventMap events;
		Position pos;

		float balance;

		void DepsawnCreatureByEntry(uint32 entry)
		{
			units.clear();
			me->GetCreatureListWithEntryInGrid(units, entry, 200.0f);
			if (!units.empty())
				for (std::list<Creature*>::iterator itr = units.begin(); itr != units.end(); ++itr)
					(*itr)->DespawnOrUnsummon();
		}

		void ClearSpine()
		{
			DepsawnCreatureByEntry(NPC_SPINE_AMALGAMATION);
			DepsawnCreatureByEntry(NPC_SPINE_BLOOD);
			DepsawnCreatureByEntry(NPC_SPINE_CORRUPTION);
			DepsawnCreatureByEntry(NPC_SPINE_TENDONS);
			DepsawnCreatureByEntry(NPC_SPINE_SPAWNER);
		}

		void ClearPlates()
		{
			if (plate = me->FindNearestGameObject(DWR_PLATE_1, 200.f)) { plate->SetGoState(GO_STATE_READY); me->CastSpell(plate, SPELL_SEAL_ARMOR_BREACH_LEFT, true); }
			if (plate = me->FindNearestGameObject(DWR_PLATE_2, 200.f)) { plate->SetGoState(GO_STATE_READY); me->CastSpell(plate, SPELL_SEAL_ARMOR_BREACH_LEFT, true); }
			if (plate = me->FindNearestGameObject(DWR_PLATE_3, 200.f)) { plate->SetGoState(GO_STATE_READY); me->CastSpell(plate, SPELL_SEAL_ARMOR_BREACH_LEFT, true); }
		}

		void Reset()
		{
			_Reset();
		}

		void EnterCombat(Unit* who)
		{
			_EnterCombat();

			events.Reset();
			events.ScheduleEvent(EVENT_SPAWNER_BLOOD, urand(8000, 12000));
			events.ScheduleEvent(EVENT_SPINE_BALANCE_CHECK, 3000);

			for (int i = 0; i < 4; i++)
			{
				me->SummonCreature(NPC_SPINE_CORRUPTION, SpawnerPositions[i]);
			}

			instance->SetData(DATA_DWR_TENDON_LEFT_HEALTH, 0);
			instance->SetData(DATA_DWR_TENDON_RIGHT_HEALTH, 0);
			instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
		}

		void EnterEvadeMode()
		{
			_EnterEvadeMode();

			ClearSpine();
			ClearPlates();

			instance->SetBossState(NPC_DEATHWING_SPINE, FAIL);
			instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
		}

		void JustDied(Unit* /*killer*/)
		{
			_JustDied();
			instance->SetBossState(NPC_DEATHWING_SPINE, DONE);
			instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
		}

		void DoAction(const int32 action)
		{
			switch (action)
			{
			case ACTION_CLEAR_UNITS:
				ClearSpine();
				break;
			case ACTION_CLEAR_PLATES:
				ClearPlates();
				break;
			case ACTION_FINISH_HIM:
				instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DEGRADATION);
				instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BLOOD_OF_NELTHARION);

				std::list<Player*> players;
				Trinity::AnyPlayerInObjectRangeCheck checker(me, 200.0f);
				Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, players, checker);
				me->VisitNearbyWorldObject(200.0f, searcher);
				for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
				{
					(*itr)->SendMovieStart(75);
					(*itr)->TeleportTo(967, -12120.0f, 12170.0f, 28.0f, 5.8f);
					(*itr)->CastSpell((*itr), VEHICLE_SPELL_PARACHUTE, true);
				}

				ClearSpine();
				me->Kill(me);
				break;
			}
		}

		float CheckBalance()
		{
			std::list<Player*> players;
			Trinity::AnyPlayerInObjectRangeCheck checker(me, 200.0f);
			Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, players, checker);
			me->VisitNearbyWorldObject(200.0f, searcher);
			if (!players.empty())
			{
				float balance = 0.0f;
				for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
					if (!(*itr)->isGameMaster() && (*itr)->GetPositionZ() < 270 && (*itr)->GetPositionY() > -13690)
						balance += (*itr)->GetPositionX() + 13855.0f;

				balance /= players.size();
				return balance;
			}

			return 0.0f;
		}

		void SendEmoteToZone(const char *text)
		{
			std::list<Player*> players;
			Trinity::AnyPlayerInObjectRangeCheck checker(me, 200.0f);
			Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, players, checker);
			me->VisitNearbyWorldObject(200.0f, searcher);
			if (!players.empty())
				for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
					(*itr)->GetSession()->SendAreaTriggerMessage(text);
		}

		void UpdateAI(uint32 diff)
		{
			if (!UpdateVictim())
				return;

			events.Update(diff);
			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_SPAWNER_BLOOD:
					units.clear();
					me->GetCreatureListWithEntryInGrid(units, NPC_SPINE_SPAWNER, 200.0f);
					if (!units.empty() && units.size() > 0)
					{
						std::list<Creature*>::iterator itr = units.begin();
						std::advance(itr, urand(0, units.size() - 1));
						if ((*itr))
						{
							(*itr)->GetPosition(&pos);
							me->SummonCreature(NPC_SPINE_BLOOD, pos);
						}
					}

					events.ScheduleEvent(EVENT_SPAWNER_BLOOD, urand(5000, 10000));
					break;
				case EVENT_SPINE_BALANCE_CHECK:
					balance = CheckBalance();

					if (abs(balance) > 6)
					{
						SendEmoteToZone(balance > 0 ? "Смертокрыл чувствует игроков на правой стороне!" : "Смертокрыл чувствует игроков на левой стороне!");
						events.ScheduleEvent(EVENT_SPINE_ROLL, 8000);
						break;
					}

					events.ScheduleEvent(EVENT_SPINE_BALANCE_CHECK, 3000);
					break;
				case EVENT_SPINE_ROLL:
					balance = CheckBalance();

					if (abs(balance) > 8)
					{
						SendEmoteToZone("*Смертокрыл делает бочку*");

						std::list<Player*> players;
						Trinity::AnyPlayerInObjectRangeCheck checker(me, 200.0f);
						Trinity::PlayerListSearcher<Trinity::AnyPlayerInObjectRangeCheck> searcher(me, players, checker);
						me->VisitNearbyWorldObject(200.0f, searcher);
						if (!players.empty())
							for (std::list<Player*>::iterator itr = players.begin(); itr != players.end(); ++itr)
							{
								if (!(*itr)->isGameMaster() && (*itr)->GetPositionZ() < 270 && (*itr)->GetPositionY() > -13690 && (!(*itr)->GetAura(SPELL_FIERY_GRIP) && !(*itr)->GetAura(AURA_GASPING_TENDRILS)))
								{
									(*itr)->GetMotionMaster()->Clear();
									(*itr)->GetMotionMaster()->MoveJump((*itr)->GetPositionX() + (((*itr)->GetPositionX() + 13855 > 0) ? irand(70, 80) : irand(-80, -70)), (*itr)->GetPositionY() - 100, (*itr)->GetPositionZ() - urand(120, 170), 35.0f, 50.0f);
								}
							}

						units.clear();
						me->GetCreatureListWithEntryInGrid(units, NPC_SPINE_AMALGAMATION, 200.0f);
						if (!units.empty())
							for (std::list<Creature*>::iterator itr = units.begin(); itr != units.end(); ++itr)
							{
								(*itr)->GetMotionMaster()->Clear();
								(*itr)->GetMotionMaster()->MoveJump((*itr)->GetPositionX() + (((*itr)->GetPositionX() + 13855 > 0) ? irand(70, 80) : irand(-80, -70)), (*itr)->GetPositionY() - 100, (*itr)->GetPositionZ() - urand(120, 170), 35.0f, 50.0f);
								(*itr)->DespawnOrUnsummon(5000);
							}

						events.ScheduleEvent(EVENT_SPINE_BALANCE_CHECK, 20000);
						break;
					}

					events.ScheduleEvent(EVENT_SPINE_BALANCE_CHECK, 3000);
					break;
				}
			}
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new boss_deathwing_spineAI(creature);
	}
};

// Corruption
// 56161
class npc_corruption : public CreatureScript
{
public:
	npc_corruption() : CreatureScript("npc_deathwing_spine_corruption") { }

	struct npc_corruptionAI : public ScriptedAI {
		npc_corruptionAI(Creature* creature) : ScriptedAI(creature)
		{
			instance = me->GetInstanceScript();
			health = 0;
			m_lastspell = 0;
		}

		InstanceScript* instance;
		Unit* target;
		EventMap events;
		Position pos;
		int health;
		uint32 m_lastspell;

		void Reset()
		{
			m_lastspell = 0;
			me->SetInCombatWithZone();

			if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
				me->Attack(target, true);
		}

		void EnterCombat(Unit* /*who*/)
		{
			m_lastspell = 0;
			events.Reset();
			events.ScheduleEvent(EVENT_CORRUPTION_GRIP, urand(10000, 35000));
			events.ScheduleEvent(EVENT_CORRUPTION_PLASMA_1, urand(2000, 28000));
		}

		void DamageTaken(Unit* /*attacker*/, uint32 &damage)
		{
			if (me->HasUnitState(UNIT_STATE_CASTING) && GetLastSpell() == SPELL_FIERY_GRIP && (health - me->GetHealth() + damage) > me->GetMaxHealth()*0.2)
			{
				me->CastStop();
			}
		}

		void JustDied(Unit* /*killer*/)
		{
			if (!me->FindNearestCreature(NPC_SPINE_CORRUPTION, 200.f, true))
			{
				std::list<Creature*> units;
				me->GetCreatureListWithEntryInGrid(units, NPC_SPINE_SPAWNER, 200.0f);
				if (!units.empty() && units.size() > 0)
				{
					std::list<Creature*>::iterator itr = units.begin();
					std::advance(itr, urand(0, units.size() - 1));
					if ((*itr))
					{
						(*itr)->GetPosition(&pos);
						(*itr)->DespawnOrUnsummon();
						me->SummonCreature(NPC_SPINE_CORRUPTION, pos);
					}
				}
			}

			me->GetPosition(&pos);
			me->SummonCreature(NPC_SPINE_SPAWNER, pos);
			me->SummonCreature(NPC_SPINE_AMALGAMATION, pos);
			me->DespawnOrUnsummon(3000);
		}

		void UpdateAI(uint32 diff)
		{
			if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
				return;

			events.Update(diff);
			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId) {
				case EVENT_CORRUPTION_PLASMA_1:
					if (target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true, -SPELL_SEARING_PLASMA_2))
					{
						me->CastSpell(target, SPELL_SEARING_PLASMA_1, false);
						SetLastSpell(SPELL_SEARING_PLASMA_1);
						events.ScheduleEvent(EVENT_CORRUPTION_PLASMA_2, 1);
						break;
					}

					events.ScheduleEvent(EVENT_CORRUPTION_PLASMA_1, urand(2000, 8000));
					break;
				case EVENT_CORRUPTION_PLASMA_2:
					me->CastSpell(target, SPELL_SEARING_PLASMA_2, false);
					SetLastSpell(SPELL_SEARING_PLASMA_2);

					events.ScheduleEvent(EVENT_CORRUPTION_PLASMA_1, urand(8000, 25000));
					break;
				case EVENT_CORRUPTION_GRIP:
					if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
					{
						health = me->GetHealth();
						me->CastSpell(target, SPELL_FIERY_GRIP, false);
						SetLastSpell(SPELL_FIERY_GRIP);
						events.ScheduleEvent(EVENT_CORRUPTION_GRIP, urand(30000, 60000));
						break;
					}

					events.ScheduleEvent(EVENT_CORRUPTION_GRIP, urand(10000, 30000));
					break;
				}
			}

			DoMeleeAttackIfReady();
		}

		uint32 GetLastSpell() const { return m_lastspell; }
		void SetLastSpell(uint32 lastspell) { m_lastspell = lastspell; }
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_corruptionAI(creature);
	}
};

// Hideos Amalgamation
// 53890
class npc_amalgamation : public CreatureScript
{
public:
	npc_amalgamation() : CreatureScript("npc_deathwing_spine_amalgamation") { }

	struct npc_amalgamationAI : public ScriptedAI {
		npc_amalgamationAI(Creature* creature) : ScriptedAI(creature)
		{
			instance = me->GetInstanceScript();
			explode = false;
		}

		InstanceScript* instance;
		Position pos;
		bool explode;

		void Reset()
		{
			me->SetInCombatWithZone();
			me->AddAura(SPELL_ABSORBED_BLOOD_BAR, me);

			if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
			{
				me->Attack(target, true);
				me->GetMotionMaster()->MoveChase(target);
			}
		}

		void DamageTaken(Unit* /*attacker*/, uint32 &damage)
		{
			if (me->HasAura(SPELL_SUPERHEATED_NUCLEUS) && me->GetHealth() <= damage)
			{
				damage = me->GetHealth() - 1;

				if (!explode)
				{
					me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
					me->CastSpell(me, SPELL_NUCLEAR_BLAST, false);
					explode = true;
				}
			}
		}

		void SpellHit(Unit* /*attacker*/, SpellInfo const* spell)
		{
			if (spell->Id == SPELL_ABSORBED_BLOOD) {
				if (Aura* aura = me->GetAura(SPELL_ABSORBED_BLOOD)) {
					me->SetPower(POWER_ALTERNATE_POWER, aura->GetStackAmount());
					if (aura->GetStackAmount() == 9 && !me->HasAura(SPELL_SUPERHEATED_NUCLEUS))
						me->CastSpell(me, SPELL_SUPERHEATED_NUCLEUS, true);
				}
			}
		}

		void JustDied(Unit* /*killer*/)
		{
			if (me->GetMap()->IsHeroic())
			{
				Unit* target = NULL;
				std::list<Player*> players;
				AmalgamationPlayersCheck check(me);
				Trinity::PlayerListSearcher<AmalgamationPlayersCheck> searcher(me, players, check);
				me->VisitNearbyObject(200.0f, searcher);
				if (!players.empty())
					me->AddAura(SPELL_CORRUPTION_DEATH, Trinity::Containers::SelectRandomContainerElement(players));

				me->CastSpell(me, SPELL_DEGRADATION, true);
			}

			me->DespawnOrUnsummon(5000);
		}

		void UpdateAI(uint32 diff)
		{
			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			if (explode)
			{
				GameObject* plate = me->FindNearestGameObject(DWR_PLATE_3, 20.0f);
				if (!plate) plate = me->FindNearestGameObject(DWR_PLATE_2, 20.0f);
				if (!plate) plate = me->FindNearestGameObject(DWR_PLATE_1, 20.0f);

				if (plate && plate->GetGoState() != GO_STATE_ACTIVE && !me->FindNearestCreature(NPC_SPINE_TENDONS, 200.0f, true))
				{
					me->CastSpell(plate, me->m_positionX < -13855 ? SPELL_BREACH_ARMOR_LEFT : SPELL_BREACH_ARMOR_RIGHT, true);
					plate->SetGoState(GO_STATE_READY);
					me->SummonCreature(NPC_SPINE_TENDONS, plate->m_positionX + (me->m_positionX < -13855 ? -7 : 7), plate->m_positionY - 9, plate->m_positionZ - 1);
				}

				me->Kill(me);
			}

			DoMeleeAttackIfReady();
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_amalgamationAI(creature);
	}
};

// Corrupted Blood
// 53889
class npc_blood : public CreatureScript
{
public:
	npc_blood() : CreatureScript("npc_deathwing_spine_blood") { }

	struct npc_bloodAI : public ScriptedAI {
		npc_bloodAI(Creature* creature) : ScriptedAI(creature)
		{
			instance = me->GetInstanceScript();
			absorbed = false;
		}

		InstanceScript* instance;
		EventMap events;
		Position pos;
		Creature* spawner;
		bool absorbed;
		bool isDead;

		void Reset()
		{
			events.Reset();
			isDead = false;
			me->SetSpeed(MOVE_RUN, 0.714286f);
			me->SetInCombatWithZone();

			if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200, true))
			{
				me->Attack(target, true);
				me->GetMotionMaster()->MoveChase(target);
			}
		}

		void JustDied(Unit* /*killer*/)
		{
			if (spawner = me->FindNearestCreature(NPC_SPINE_SPAWNER, 100.0f, true))
			{
				isDead = true;
				me->CombatStop();
				me->setDeathState(ALIVE);
				me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
				me->SetReactState(REACT_PASSIVE);
				events.ScheduleEvent(EVENT_BLOOD_SPAWNER, 3000);
			}
			else
			{
				me->DespawnOrUnsummon(3000);
			}

			me->CastSpell(me, SPELL_BURST, true);
		}

		void MovementInform(uint32 /*type*/, uint32 id)
		{
			if (id == 1337)
			{
				me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
				me->SetReactState(REACT_AGGRESSIVE);
				me->Respawn(true);
				Reset();
			}
		}

		void UpdateAI(uint32 diff)
		{
			if (absorbed)
				return;

			if (isDead)
			{
				if (Creature* amalgamation = me->FindNearestCreature(NPC_SPINE_AMALGAMATION, 5.0f, true))
				{
					me->CastSpell(amalgamation, SPELL_ABSORBED_BLOOD, true);
					me->DespawnOrUnsummon(3000);
					absorbed = true;
					return;
				}
			}

			events.Update(diff);
			while (uint32 eventId = events.ExecuteEvent())
			{
				switch (eventId)
				{
				case EVENT_BLOOD_SPAWNER:
					spawner->GetPosition(&pos);
					me->SetSpeed(MOVE_RUN, 0.07f);
					me->GetMotionMaster()->MovePoint(1337, pos);
					break;
				}
			}

			DoMeleeAttackIfReady();
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_bloodAI(creature);
	}
};

// Burning tendon's
// 56575
class npc_tendons : public CreatureScript
{
public:
	npc_tendons() : CreatureScript("npc_deathwing_spine_tendons") { }

	struct npc_tendonsAI : public ScriptedAI
	{
		npc_tendonsAI(Creature* creature) : ScriptedAI(creature)
		{
			instance = me->GetInstanceScript();
			cast = false;
		}

		InstanceScript* instance;
		Position pos;
		bool cast;

		void JustDied(Unit* /*killer*/)
		{
			if (Creature* spine = me->FindNearestCreature(NPC_DEATHWING_SPINE, 200.0f))
			{
				GameObject* plate = me->FindNearestGameObject(DWR_PLATE_3, 15.0f);
				if (!plate) plate = me->FindNearestGameObject(DWR_PLATE_2, 15.0f);
				if (!plate) plate = me->FindNearestGameObject(DWR_PLATE_1, 15.0f);

				if (plate)
				{
					me->CastSpell(plate, me->m_positionX < -13855 ? SPELL_PLATE_FLY_OFF_LEFT : SPELL_PLATE_FLY_OFF_RIGHT, true);
					plate->SetGoState(GO_STATE_ACTIVE);
					switch (plate->GetEntry())
					{
					case DWR_PLATE_1:
						spine->SetHealth(spine->GetMaxHealth()*0.66);
						for (int i = 4; i < 6; i++)
							me->SummonCreature(NPC_SPINE_CORRUPTION, SpawnerPositions[i]);
						break;
					case DWR_PLATE_2:
						spine->SetHealth(spine->GetMaxHealth()*0.33);
						for (int i = 6; i < 8; i++)
							me->SummonCreature(NPC_SPINE_CORRUPTION, SpawnerPositions[i]);
						break;
					case DWR_PLATE_3:
						spine->AI()->DoAction(ACTION_FINISH_HIM);
						break;
					}
				}
			}

			if (instance)
			{
				instance->SetData(DATA_DWR_TENDON_LEFT_HEALTH, 0);
				instance->SetData(DATA_DWR_TENDON_RIGHT_HEALTH, 0);
				instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
			}

			me->DespawnOrUnsummon();
		}

		void UpdateAI(uint32 diff)
		{
			if (me->HasUnitState(UNIT_STATE_CASTING))
				return;

			if (!cast)
			{
				cast = true;

				if (instance)
				{
					uint32 health = instance->GetData(me->m_positionX < -13855 ? DATA_DWR_TENDON_LEFT_HEALTH : DATA_DWR_TENDON_RIGHT_HEALTH);
					if (health > 0)
						me->SetHealth(health);
					instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
				}

				me->m_checkLoS = false;

				me->SetInCombatWithZone();

				GameObject* plate = me->FindNearestGameObject(DWR_PLATE_3, 15.0f);
				if (!plate) plate = me->FindNearestGameObject(DWR_PLATE_2, 15.0f);
				if (!plate) plate = me->FindNearestGameObject(DWR_PLATE_1, 15.0f);

				if (plate)
				{
					me->CastSpell(plate, me->m_positionX < -13855 ? SPELL_SEAL_ARMOR_BREACH_LEFT : SPELL_SEAL_ARMOR_BREACH_RIGHT, false);
					plate->SetGoState(GO_STATE_READY);
				}

				return;
			}

			if (instance)
			{
				instance->SetData(me->m_positionX < -13855 ? DATA_DWR_TENDON_LEFT_HEALTH : DATA_DWR_TENDON_RIGHT_HEALTH, me->GetHealth());
				instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
			}

			me->DespawnOrUnsummon();
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_tendonsAI(creature);
	}
};

// Spawner
// 53888
class npc_spawner : public CreatureScript
{
public:
	npc_spawner() : CreatureScript("npc_deathwing_spine_spawner") { }

	struct npc_spawnerAI : public ScriptedAI {
		npc_spawnerAI(Creature* creature) : ScriptedAI(creature) {  }

		void Reset()
		{
			me->CastSpell(me, SPELL_GRASPING_TENDRILS, true);
			me->SetInCombatWithZone();
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new npc_spawnerAI(creature);
	}
};

// Blood Corruption: Death & Earth
// 106199, 106200
class spell_blood_of_corruption : public SpellScriptLoader
{
public:
	spell_blood_of_corruption() : SpellScriptLoader("spell_blood_of_corruption") { }

	class spell_blood_of_corruption_AuraScript : public AuraScript
	{
		PrepareAuraScript(spell_blood_of_corruption_AuraScript);

		void HandleDispel(DispelInfo* dispelInfo)
		{
			uint32 cur_duration = GetDuration();
			if (Unit* owner = GetUnitOwner())
			{
				Unit* target = NULL;
				std::list<Player*> players;
				AmalgamationPlayersCheck check(owner);
				Trinity::PlayerListSearcher<AmalgamationPlayersCheck> searcher(owner, players, check);
				owner->VisitNearbyObject(200.0f, searcher);
				if (!players.empty())
					target = Trinity::Containers::SelectRandomContainerElement(players);
				else
					target = owner;

				if (target)
					if (Aura* aur = owner->AddAura((roll_chance_i(40) ? SPELL_CORRUPTION_EARTH : SPELL_CORRUPTION_DEATH), target))
						aur->SetDuration(cur_duration);
			}
		}

		void HandleRemove(AuraEffect const* aurEff, AuraEffectHandleModes mode)
		{
			if (Unit* owner = GetUnitOwner())
			{
				AuraRemoveMode removeMode = GetTargetApplication()->GetRemoveMode();
				if (removeMode == AURA_REMOVE_BY_EXPIRE || removeMode == AURA_REMOVE_BY_DEATH)
					owner->CastSpell(owner, (GetId() == SPELL_CORRUPTION_EARTH) ? SPELL_BLOOD_OF_NELTHARION : SPELL_BLOOD_OF_DEATHWING, true);
			}
		}

		void Register()
		{
			AfterDispel += AuraDispelFn(spell_blood_of_corruption_AuraScript::HandleDispel);
			AfterEffectRemove += AuraEffectRemoveFn(spell_blood_of_corruption_AuraScript::HandleRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
		}
	};

	AuraScript* GetAuraScript() const
	{
		return new spell_blood_of_corruption_AuraScript();
	}
};

void AddSC_boss_deathwing_spine()
{
	new boss_deathwing_spine();
	new npc_corruption();
	new npc_amalgamation();
	new npc_blood();
	new npc_tendons();
	new npc_spawner();
	new spell_blood_of_corruption();
}