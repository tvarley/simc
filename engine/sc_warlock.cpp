// ==========================================================================
// Dedmonwakeen's DPS-DPM Simulator.
// Send questions to natehieter@gmail.com
// ==========================================================================
#include "simulationcraft.h"


/*
 *
 * To Do:
 * - Check if curse_of_elements is counted for affliction_effects in every possible situation
 *  (optimal_raid=1, or with multiple wl's, dk's/druis), it enhances drain soul damage.
 * - Burning Embers
 * - Hand of Gul'dan Dot Effect
 * - Demonic Pact Change to constant aura activated by pet presence
 * - Demon Soul
 * - Dark Intent: Add Callback Action on the Dark Intent Target
 * - Dark Intent: Change Callback to dot-crit result.
 *
 *
 * Cata log:
 * - started to migracte action_t* active_* to dot_t*
 * - Added new talent_t*
 * - begun removing old talents, changing spells
 *
 *
 *
 */


// ==========================================================================
// Warlock
// ==========================================================================

enum stone_type_t { STONE_NONE=0, SPELL_STONE, FIRE_STONE };

struct warlock_pet_t;

struct warlock_t : public player_t
{
  // Active
  warlock_pet_t* active_pet;

  dot_t*	dots_corruption;
  dot_t*	dots_unstable_affliction;
  dot_t*	dots_bane_of_agony;
  dot_t*	dots_bane_of_doom;
  dot_t*	dots_immolate;
  dot_t* 	dots_drain_life;
  dot_t*	dots_drain_soul;
  dot_t*	dots_shadowflame;
  dot_t*	dots_curse_of_elements;
  dot_t*	dots_burning_embers;

  buff_t* buffs_backdraft;
  buff_t* buffs_decimation;
  buff_t* buffs_demonic_empowerment;
  buff_t* buffs_demonic_frenzy;
  buff_t* buffs_empowered_imp;
  buff_t* buffs_eradication;
  buff_t* buffs_fel_armor;
  buff_t* buffs_haunted;
  buff_t* buffs_life_tap_glyph;
  buff_t* buffs_shadow_embrace;
  buff_t* buffs_metamorphosis;
  buff_t* buffs_molten_core;
  buff_t* buffs_shadow_trance;
  buff_t* buffs_tier7_2pc_caster;
  buff_t* buffs_tier7_4pc_caster;
  buff_t* buffs_tier10_4pc_caster;
  buff_t* buffs_hand_of_guldan;
  buff_t* buffs_improved_soul_fire;
  buff_t* buffs_dark_intent_feedback;

  cooldown_t* cooldowns_improved_soul_fire;

  spell_t* spells_burning_embers;



  // Affliction
  talent_t* talent_doom_and_gloom; // done
  talent_t* talent_improved_life_tap; // done
  talent_t* talent_improved_corruption; // done
  talent_t* talent_jinx;
  talent_t* talent_soul_siphon; // done
  talent_t* talent_siphon_life; // done
  talent_t* talent_eradication; // done
  talent_t* talent_soul_swap;
  talent_t* talent_shadow_embrace; // done
  talent_t* talent_deaths_embrace; // done
  talent_t* talent_nightfall; // done
  talent_t* talent_soulburn_seed_of_corruption;
  talent_t* talent_everlasting_affliction; // done
  talent_t* talent_pandemic; // done
  talent_t* talent_haunt; // done

  // Demonology
  talent_t* talent_demonic_embrace; // done
  talent_t* talent_dark_arts; // done
  talent_t* talent_mana_feed; // done
  talent_t* talent_demonic_aegis;
  talent_t* talent_impending_doom;
  talent_t* talent_demonic_empowerment;
  talent_t* talent_molten_core; // done
  talent_t* talent_hand_of_guldan;
  talent_t* talent_ancient_grimoire;
  talent_t* talent_nemesis; // done
  talent_t* talent_inferno;
  talent_t* talent_decimation; // done
  talent_t* talent_cremation;
  talent_t* talent_demonic_pact;
  talent_t* talent_metamorphosis; // done

  // Destruction
  talent_t* talent_bane; // done
  talent_t* talent_shadow_and_flame; // done
  talent_t* talent_improved_immolate;
  talent_t* talent_improved_soul_fire;
  talent_t* talent_emberstorm; // done
  talent_t* talent_improved_searing_pain;
  talent_t* talent_backdraft; // done
  talent_t* talent_shadowburn; // done
  talent_t* talent_burning_embers;
  talent_t* talent_soul_leech; // done
  talent_t* talent_fire_and_brimstone; // done
  talent_t* talent_shadowfury; // done
  talent_t* talent_empowered_imp; // done
  talent_t* talent_bane_of_havoc;
  talent_t* talent_chaos_bolt; // done



  struct talent_spec_spells_t
  {
    talent_spec_spell_id_t* unstable_affliction;
    talent_spec_spell_id_t* shadow_mastery;
    talent_spec_spell_id_t* summon_felguard;
    talent_spec_spell_id_t* master_demonologist;
    talent_spec_spell_id_t* conflagrate;
    talent_spec_spell_id_t* cataclysm;
  };
  talent_spec_spells_t talent_spec_spells;

  struct mastery_spells_t
  {
    mastery_spell_id_t* potent_afflictions;
    mastery_spell_id_t* master_demonologist;
    mastery_spell_id_t* fiery_apocalypse;
  };
  mastery_spells_t mastery_spells;

  struct class_spells_t
   {

   };
   class_spells_t class_spells;

  std::string dark_intent_target_str;

  // Gains
  gain_t* gains_dark_pact;
  gain_t* gains_fel_armor;
  gain_t* gains_felhunter;
  gain_t* gains_life_tap;
  gain_t* gains_soul_leech;

  // Procs
  proc_t* procs_empowered_imp;

  // Random Number Generators
  rng_t* rng_soul_leech;
  rng_t* rng_everlasting_affliction;
  rng_t* rng_pandemic;
  rng_t* rng_cremation;

  // Custom Parameters
  std::string summon_pet_str;




  struct glyphs_t
  {
    int chaos_bolt;
    int conflagrate;
    int corruption;
    int bane_of_agony;
    int felguard;
    int felhunter;
    int haunt;
    int immolate;
    int imp;
    int incinerate;
    int life_tap;
    int metamorphosis;
    int searing_pain;
    int shadow_bolt;
    int shadow_burn;
    int siphon_life;
    int unstable_affliction;
    int quick_decay;
    glyphs_t() { memset( ( void* ) this, 0x0, sizeof( glyphs_t ) ); }
  };
  glyphs_t glyphs;


  warlock_t( sim_t* sim, const std::string& name, race_type r = RACE_NONE ) : player_t( sim, WARLOCK, name, r )
  {
    distance = 40;


    active_pet                 			= 0;

    dots_corruption 					= get_dot( "corruption" );
    dots_unstable_affliction 			= get_dot( "unstable_affliction" );
    dots_bane_of_agony 					= get_dot( "bane_of_agony" );
    dots_bane_of_doom 					= get_dot( "bane_of_doom" );
    dots_drain_life 					= get_dot( "drain_life" );
    dots_drain_soul 					= get_dot( "drain_soul" );
    dots_shadowflame 					= get_dot( "shadowflame" );
    dots_immolate 						= get_dot( "immolate" );
    dots_curse_of_elements 				= get_dot( "curse_of_elements" );
    dots_burning_embers 				= get_dot( "burning_embers" );

    cooldowns_improved_soul_fire 		= get_cooldown ("improved_soul_fire");
    cooldowns_improved_soul_fire -> duration = 30.0;


    // Talents

    // Core
    // Affliction
    talent_spec_spells.unstable_affliction 	= new talent_spec_spell_id_t ( this, "unstable_affliction", "Unstable Affliction", WARLOCK_AFFLICTION );
    talent_spec_spells.shadow_mastery 		= new talent_spec_spell_id_t ( this, "shadow_mastery", "Shadow Mastery", WARLOCK_AFFLICTION );
    // Demonology
    talent_spec_spells.summon_felguard 		= new talent_spec_spell_id_t ( this, "summon_felguard", "Summon Felguard", WARLOCK_DEMONOLOGY );
    talent_spec_spells.master_demonologist 	= new talent_spec_spell_id_t ( this, "master_demonologist", "Master Demonologist", WARLOCK_DEMONOLOGY );
    //Destruction
    talent_spec_spells.conflagrate 			= new talent_spec_spell_id_t ( this, "conflagrate", "Conflagrate", WARLOCK_DESTRUCTION );
    talent_spec_spells.cataclysm 			= new talent_spec_spell_id_t ( this, "cataclysm", "Cataclysm", WARLOCK_DESTRUCTION );

    mastery_spells.fiery_apocalypse 		= new mastery_spell_id_t ( this, "fiery_apocalypse", "Fiery Apocalypse", WARLOCK_DESTRUCTION );
    mastery_spells.potent_afflictions 		= new mastery_spell_id_t (this, "poten_afflictions", "Potent Afflictions", WARLOCK_AFFLICTION );
    mastery_spells.master_demonologist 		= new mastery_spell_id_t ( this, "master_demonologist", "Master Demonologist", WARLOCK_DEMONOLOGY );

    // Affliction
    talent_doom_and_gloom 				= new talent_t ( this, "doom_and_gloom", "Doom and Gloom" );
    talent_improved_life_tap 			= new talent_t ( this, "improved_life_tap", "Improved Life Tap" );
    talent_improved_corruption 			= new talent_t ( this, "improved_corruption", "Improved Corruption" );
    talent_jinx 						= new talent_t ( this, "jinx", "Jinx" );
    talent_soul_siphon 					= new talent_t ( this, "soul_siphon", "Soul Siphon" );
    talent_siphon_life 					= new talent_t ( this, "siphon_life", "Siphon Life" );
    talent_eradication 					= new talent_t ( this, "eradication", "Eradication" );
    talent_soul_swap 					= new talent_t ( this, "soul_swap", "Soul Swap" );
    talent_shadow_embrace 				= new talent_t ( this, "shadow_embrace", "Shadow Embrace" );
    talent_deaths_embrace 				= new talent_t ( this, "deaths_embrace", "Death's Embrace" );
    talent_nightfall 					= new talent_t ( this, "nightfall", "Nightfall" );
    talent_soulburn_seed_of_corruption 	= new talent_t ( this, "soulburn_seed_of_corruption", "Soulburn: Seed of Corruption" );
    talent_everlasting_affliction 		= new talent_t ( this, "everlasting_affliction", "Everlasting Affliction" );
    talent_pandemic 					= new talent_t ( this, "pandemic", "Pandemic" );
    talent_haunt 						= new talent_t ( this, "haunt", "Haunt" );

    // Demonology
    talent_demonic_embrace 				= new talent_t ( this, "demonic_embrace", "Demonic Embrace" );
    talent_dark_arts 					= new talent_t ( this, "dark_arts", "Dark Arts" );
    talent_mana_feed 					= new talent_t ( this, "mana_feed", "Mana Feed" );
    talent_demonic_aegis 				= new talent_t ( this, "demonic_aegis", "Demonic Aegis" );
    talent_impending_doom 				= new talent_t ( this, "impending_doom", "Impending Doom" );
    talent_demonic_empowerment 			= new talent_t ( this, "demonic_empowerment", "Demonic Empowerment" );
    talent_molten_core 					= new talent_t ( this, "molten_core", "Molten Core" );
    talent_hand_of_guldan 				= new talent_t ( this, "hand_of_guldan", "Hand of Gul'dan" );
    talent_ancient_grimoire 			= new talent_t ( this, "ancient_grimoire", "Ancient Grimoire" );
    talent_nemesis 						= new talent_t ( this, "nemesis", "Nemesis" );
    talent_inferno 						= new talent_t ( this, "inferno", "Inferno" );
    talent_decimation 					= new talent_t ( this, "decimation", "Decimation" );
    talent_cremation 					= new talent_t ( this, "cremation", "Cremation" );
    talent_demonic_pact 				= new talent_t ( this, "demonic_pact", "Demonic Pact" );
    talent_metamorphosis 				= new talent_t ( this, "metamorphosis", "Metamorphosis" );

        // Destruction
    talent_bane 						= new talent_t ( this, "bane", "Bane" );
    talent_shadow_and_flame 			= new talent_t ( this, "shadow_and_flame", "Shadow and Flame" );
    talent_improved_immolate 			= new talent_t ( this, "improved_immolate", "Improved Immolate" );
    talent_improved_soul_fire 			= new talent_t ( this, "improved_soul_fire", "Improved Soul Fire" );
    talent_emberstorm 					= new talent_t ( this, "emberstorm", "Emberstorm" );
    talent_improved_searing_pain 		= new talent_t ( this, "improved_searing_pain", "Improved Searing Pain" );
    talent_backdraft 					= new talent_t ( this, "backdraft", "Backdraft" );
    talent_shadowburn 					= new talent_t ( this, "shadowburn", "Shadowburn" );
    talent_burning_embers 				= new talent_t ( this, "burning_embers", "Burning Embers" );
    talent_soul_leech 					= new talent_t ( this, "soul_leech", "Soul Leech" );
    talent_fire_and_brimstone 			= new talent_t ( this, "fire_and_brimstone", "Fire and Brimstone" );
    talent_shadowfury 					= new talent_t ( this, "shadowfury", "Shadowfury" );
    talent_empowered_imp 				= new talent_t ( this, "empowered_imp", "Empowered Imp" );
    talent_bane_of_havoc 				= new talent_t ( this, "bane_of_havoc", "Bane of Havoc" );
    talent_chaos_bolt 					= new talent_t ( this, "chaos_bolt", "Chaos Bolt" );
  }

  // Character Definition
  virtual void      init_glyphs();
  virtual void      init_race();
  virtual void      init_base();
  virtual void      init_scaling();
  virtual void      init_buffs();
  virtual void      init_gains();
  virtual void      init_uptimes();
  virtual void      init_procs();
  virtual void      init_rng();
  virtual void      init_talents();
  virtual void		init_spells();
  virtual void      init_actions();
  virtual void      reset();
  virtual std::vector<talent_translation_t>& get_talent_list();
  virtual std::vector<option_t>& get_options();
  virtual action_t* create_action( const std::string& name, const std::string& options );
  virtual pet_t*    create_pet   ( const std::string& name );
  virtual void      create_pets();
  virtual int       decode_set( item_t& item );
  virtual int       primary_resource() SC_CONST { return RESOURCE_MANA; }
  virtual int       primary_role() SC_CONST     { return ROLE_SPELL; }
  virtual talent_tree_type primary_tree() SC_CONST;
  virtual double    composite_spell_power( int school ) SC_CONST;
  virtual double	composite_spell_haste() SC_CONST;




  // Event Tracking
  virtual void regen( double periodicity );
  virtual action_expr_t* create_expression( action_t*, const std::string& name );

  // Utilities
  int affliction_effects()
  {
    int effects = 0;
    if ( dots_curse_of_elements -> ticking()        ) effects++;
    if ( dots_bane_of_agony -> ticking() 			) effects++;
    if ( dots_bane_of_doom -> ticking() 			) effects++;
    if ( dots_corruption -> ticking()               ) effects++;
    if ( dots_drain_life -> ticking()              	) effects++;
    if ( dots_drain_soul -> ticking()              	) effects++;
    if ( dots_unstable_affliction -> ticking()      ) effects++;
    if ( buffs_haunted        -> check()			) effects++;
    if ( buffs_shadow_embrace -> check()			) effects++;
    return effects;
  }
  int active_dots()
  {
    int dots = 0;
    if ( dots_bane_of_agony -> ticking()            ) dots++;
    if ( dots_bane_of_doom -> ticking()             ) dots++;
    if ( dots_corruption -> ticking()               ) dots++;
    if ( dots_drain_life -> ticking()               ) dots++;
    if ( dots_drain_soul -> ticking()               ) dots++;
    if ( dots_immolate -> ticking()                 ) dots++;
    if ( dots_shadowflame -> ticking()              ) dots++;
    if ( dots_unstable_affliction -> ticking()      ) dots++;
    return dots;
  }
};

// ==========================================================================
// Curse of Elements
// ==========================================================================

struct coe_debuff_t : public debuff_t
{
  coe_debuff_t( sim_t* s ) : debuff_t( s, "curse_of_elements", 1, 300.0 ) {}
  virtual void expire()
  {
    if( player ) 
    {
      player = 0;
    }
    debuff_t::expire();
  }
};

// ==========================================================================
// Warlock Pet
// ==========================================================================

struct warlock_pet_t : public pet_t
{
  int       pet_type;
  double    damage_modifier;
  attack_t* melee;

  warlock_pet_t( sim_t* sim, player_t* owner, const std::string& pet_name, int pt ) :
      pet_t( sim, owner, pet_name ), pet_type( pt ), 
      damage_modifier( 1.0 ), melee( 0 )
  {
  }

  virtual bool ooc_buffs() { return true; }

  virtual void init_base()
  {
    pet_t::init_base();

    attribute_base[ ATTR_STRENGTH  ] = 314;
    attribute_base[ ATTR_AGILITY   ] =  90;
    attribute_base[ ATTR_STAMINA   ] = 328;
    attribute_base[ ATTR_INTELLECT ] = 150;
    attribute_base[ ATTR_SPIRIT    ] = 209;


    base_attack_crit = 0.0328;
    base_spell_crit  = 0.0092;
    
    initial_attack_power_per_strength = 2.0;
    initial_attack_crit_per_agility   = 0.01 / 52.0;
    initial_spell_crit_per_intellect  = owner -> initial_spell_crit_per_intellect;

    health_per_stamina = 10;
    mana_per_intellect = 10.8;
    mp5_per_intellect  = 2.0 / 3.0;

    base_mp5 = -55;
  }

  virtual void schedule_ready( double delta_time=0,
                               bool   waiting=false )
  {
    if ( melee && ! melee -> execute_event )
    {
      melee -> schedule_execute();
    }

    pet_t::schedule_ready( delta_time, waiting );
  }

  virtual void summon( double duration=0 )
  {
    warlock_t* o = owner -> cast_warlock();
    pet_t::summon( duration );
    o -> active_pet = this;
  }

  virtual void dismiss()
  {
    warlock_t* o = owner -> cast_warlock();
    pet_t::dismiss();
    o -> active_pet = 0;
  }

  virtual void interrupt()
  {
    pet_t::interrupt();
    if ( melee ) melee -> cancel();
  }

  void adjust_base_modifiers( action_t* a )
  {
    warlock_t* o = owner -> cast_warlock();

    // Orc Command Racial
    if ( o -> race == RACE_ORC )
    {
      a -> base_multiplier  *= 1.05;
    }
    if ( o -> set_bonus.tier9_2pc_caster() )
    {
      a -> base_crit += 0.10;
    }

    if ( o -> buffs_hand_of_guldan -> up() )
    {
    	a -> base_crit += 0.15;
    }
    if ( o -> mastery_spells.master_demonologist -> enabled && o -> buffs_metamorphosis -> up())
    {
    	a -> base_multiplier *= 1.0 + o -> mastery_spells.master_demonologist -> enabled * ( 0.13 + o -> composite_mastery() * 0.015 );
    }
  }



  virtual double composite_attack_hit() SC_CONST
  {
    return owner -> composite_spell_hit();
  }

  virtual double composite_attack_expertise() SC_CONST
  {
    return owner -> composite_spell_hit() * 26.0 / 17.0;
  }

  virtual void register_callbacks();

  virtual int primary_resource() SC_CONST { return RESOURCE_MANA; }
};

// ==========================================================================
// Warlock Pet Melee
// ==========================================================================

struct warlock_pet_melee_t : public attack_t
{
  warlock_pet_melee_t( player_t* player, const char* name ) :
      attack_t( name, player, RESOURCE_NONE, SCHOOL_PHYSICAL, TREE_NONE, false )
  {
    warlock_pet_t* p = ( warlock_pet_t* ) player -> cast_pet();

    weapon = &( p -> main_hand_weapon );
    base_execute_time = weapon -> swing_time;
    base_dd_min = base_dd_max = 1;
    may_crit    = true;
    background  = true;
    repeating   = true;

    p -> adjust_base_modifiers( this );

    base_multiplier *= p -> damage_modifier;
  }

  virtual void player_buff()
  {
    warlock_pet_t* p = ( warlock_pet_t* ) player -> cast_pet();
    warlock_t* o = p -> owner -> cast_warlock();
    attack_t::player_buff();
    if ( o -> buffs_tier10_4pc_caster -> up() )
    {
      player_multiplier *= 1.10;
    }
    if ( p -> pet_type != PET_INFERNAL )
    {
      player_attack_power += 0.57 * o -> composite_spell_power( SCHOOL_MAX );

    }
  }
};

// ==========================================================================
// Warlock Pet Attack
// ==========================================================================

struct warlock_pet_attack_t : public attack_t
{
  warlock_pet_attack_t( const char* n, player_t* player, int r=RESOURCE_MANA, int s=SCHOOL_PHYSICAL ) :
      attack_t( n, player, r, s, TREE_NONE, true )
  {
    special    = true;
    may_crit   = true;

  }

  virtual void execute()
  {
    attack_t::execute();
  }

  virtual void player_buff()
  {
    warlock_pet_t* p = ( warlock_pet_t* ) player -> cast_pet();
    warlock_t* o = p -> owner -> cast_warlock();
    attack_t::player_buff();
    player_attack_power += 0.57 * o -> composite_spell_power( SCHOOL_MAX );
    if ( o -> buffs_tier10_4pc_caster -> up() )
    {
      player_multiplier *= 1.10;
    }

  }

};

// ==========================================================================
// Warlock Pet Spell
// ==========================================================================

struct warlock_pet_spell_t : public spell_t
{

  warlock_pet_spell_t( const char* n, player_t* player, int r=RESOURCE_MANA, int s=SCHOOL_SHADOW ) :
      spell_t( n, player, r, s )
  {
    warlock_pet_t* p = ( warlock_pet_t* ) player -> cast_pet();
    p -> adjust_base_modifiers( this );
  }

  virtual void player_buff()
  {
    warlock_pet_t* p = ( warlock_pet_t* ) player -> cast_pet();
    warlock_t* o = p -> owner -> cast_warlock();
    spell_t::player_buff();
    if ( o -> buffs_tier10_4pc_caster -> up() )
    {
      player_multiplier *= 1.10;
    }
    player_spell_power += 0.15 * o -> composite_spell_power( SCHOOL_MAX );

  }
};

// ==========================================================================
// Pet Imp
// ==========================================================================

struct imp_pet_t : public warlock_pet_t
{
  struct fire_bolt_t : public warlock_pet_spell_t
  {
    fire_bolt_t( player_t* player ):
        warlock_pet_spell_t( "fire_bolt", player, RESOURCE_MANA, SCHOOL_FIRE )
    {
      imp_pet_t* p = ( imp_pet_t* ) player -> cast_pet();
      warlock_t* o = p -> owner -> cast_warlock();

      static rank_t ranks[] =
      {
        { 78, 8, 203, 227, 0, 180 },
        { 68, 8, 110, 124, 0, 145 },
        { 58, 7,  83,  93, 0, 115 },
        { 0, 0, 0, 0, 0, 0 }
      };
      init_rank( ranks, 47964 );


      base_execute_time = 2.5;
      direct_power_mod  = ( base_execute_time / 3.5 ); // was 2.0/3.5
      may_crit          = true;

      min_gcd           = 1.0;


      base_multiplier *= 1.0 + ( o -> talent_empowered_imp -> rank * 0.10 +
                                 o -> glyphs.imp            * 0.20 );

    }
    virtual void execute();
    virtual void player_buff()
    {
      warlock_t* o = player -> cast_pet() -> owner -> cast_warlock();
      warlock_pet_spell_t::player_buff();
      if ( o -> buffs_demonic_empowerment -> up() ) player_crit += 0.20;
    }
  };

  imp_pet_t( sim_t* sim, player_t* owner ) :
      warlock_pet_t( sim, owner, "imp", PET_IMP )
  {
    action_list_str = "fire_bolt";
  }

  virtual void init_base()
  {
    warlock_pet_t::init_base();

    attribute_base[ ATTR_STRENGTH  ] = 297;
    attribute_base[ ATTR_AGILITY   ] =  79;
    attribute_base[ ATTR_STAMINA   ] = 118;
    attribute_base[ ATTR_INTELLECT ] = 424;
    attribute_base[ ATTR_SPIRIT    ] = 367;

    resource_base[ RESOURCE_HEALTH ] = 4011;
    resource_base[ RESOURCE_MANA   ] = 1175;

    health_per_stamina = 2;
    mana_per_intellect = 4.8;
    mp5_per_intellect  = 5.0 / 6.0;

    base_mp5 = -257;
  }

  virtual action_t* create_action( const std::string& name,
                                   const std::string& options_str )
  {
    if ( name == "fire_bolt" ) return new fire_bolt_t( this );

    return player_t::create_action( name, options_str );
  }
};

// ==========================================================================
// Pet Felguard
// ==========================================================================

struct felguard_pet_t : public warlock_pet_t
{
  struct cleave_t : public warlock_pet_attack_t
  {
    cleave_t( player_t* player ) :
        warlock_pet_attack_t( "cleave", player, RESOURCE_MANA, SCHOOL_PHYSICAL )
    {
      felguard_pet_t* p = ( felguard_pet_t* ) player -> cast_pet();

      static rank_t ranks[] =
      {
        { 76, 4, 124, 124, 0, 0.1 },
        { 68, 3,  78,  78, 0, 0.1 },
        { 0, 0, 0, 0, 0, 0 }
      };
      init_rank( ranks, 47994 );

      weapon   = &( p -> main_hand_weapon );
      cooldown -> duration = 6.0;
    }
    virtual void player_buff()
    {
      felguard_pet_t* p = ( felguard_pet_t* ) player -> cast_pet();
      warlock_t*      o = p -> owner -> cast_warlock();

      warlock_pet_attack_t::player_buff();
      player_attack_power *= 1.0 + o -> buffs_demonic_frenzy -> stack() * ( 0.05  + 0.01 * o -> talent_dark_arts -> rank );
      if ( o -> glyphs.felguard ) player_attack_power *= 1.20;
    }
  };

  struct melee_t : public warlock_pet_melee_t
  {
    melee_t( player_t* player ) :
        warlock_pet_melee_t( player, "felguard_melee" )
    {}
    virtual double execute_time() SC_CONST
    {
      warlock_t* o = player -> cast_pet() -> owner -> cast_warlock();
      double t = attack_t::execute_time();
      if ( o -> buffs_demonic_empowerment -> up() ) t *= 1.0 / 1.20;
      return t;
    }

    virtual void player_buff()
    {
      felguard_pet_t* p = ( felguard_pet_t* ) player -> cast_pet();
      warlock_t*      o = p -> owner -> cast_warlock();

      warlock_pet_melee_t::player_buff();
      player_attack_power *= 1.0 + o -> buffs_demonic_frenzy -> stack() * ( 0.05 );

      if ( o -> glyphs.felguard ) player_attack_power *= 1.20;
    }
    virtual void assess_damage( double amount, int dmg_type )
    {
      warlock_t*  o = player -> cast_pet() -> owner -> cast_warlock();
      attack_t::assess_damage( amount, dmg_type );
      o -> buffs_demonic_frenzy -> trigger();
    }
  };

  felguard_pet_t( sim_t* sim, player_t* owner ) :
      warlock_pet_t( sim, owner, "felguard", PET_FELGUARD )
  {
    main_hand_weapon.type       = WEAPON_BEAST;
    main_hand_weapon.min_dmg    = 412.5;
    main_hand_weapon.max_dmg    = 412.5;
    main_hand_weapon.damage     = ( main_hand_weapon.min_dmg + main_hand_weapon.max_dmg ) / 2;
    main_hand_weapon.swing_time = 2.0;

    damage_modifier = 1.05;

    action_list_str = "cleave/wait_until_ready";
  }
  virtual void init_base()
  {
    warlock_pet_t::init_base();

    resource_base[ RESOURCE_HEALTH ] = 1627;
    resource_base[ RESOURCE_MANA   ] = 3331;

    base_attack_power = -20;

    melee = new melee_t( this );
  }
  virtual action_t* create_action( const std::string& name,
                                   const std::string& options_str )
  {
    if ( name == "cleave" ) return new cleave_t( this );

    return player_t::create_action( name, options_str );
  }
};

// ==========================================================================
// Pet Felhunter
// ==========================================================================

struct felhunter_pet_t : public warlock_pet_t
{
  // TODO: Need to add fel intelligence on the warlock while felhunter is out
  // This is +48 int / + 64 spi at rank 5, plus 5%/10% if talented in the affliction tree
  // These do NOT stack with Prayer of Spirit, or with Arcane Intellect/Arcane Brilliance
  struct shadow_bite_t : public warlock_pet_attack_t
  {
    shadow_bite_t( player_t* player ) :
        warlock_pet_attack_t( "shadow_bite", player, RESOURCE_MANA, SCHOOL_SHADOW )
    {
      felhunter_pet_t* p = ( felhunter_pet_t* ) player -> cast_pet();
      warlock_t*       o = p -> owner -> cast_warlock();

      static rank_t ranks[] =
      {
        { 74, 5, 118, 118, 0, 0.03 },
        { 66, 4, 101, 101, 0, 0.03 },
        { 0, 0, 0, 0, 0, 0 }
      };
      init_rank( ranks, 54053 );

      base_spell_power_multiplier = 1.0;
      base_attack_power_multiplier = 0.0;

      direct_power_mod  = ( 1.5 / 3.5 ); // Converting from Pet Attack Power into Pet Spell Power.
      may_crit          = true;

      cooldown -> duration = 6.0;

      target_multiplier *= 1.0 + o -> talent_dark_arts -> rank * 0.05;

      base_crit_bonus = 0.5;
      base_crit_bonus_multiplier = 2.0;
    }
      
    virtual void execute()
    {
      warlock_pet_attack_t::execute();
    }

    virtual void player_buff()
    {
      felhunter_pet_t* p = ( felhunter_pet_t* ) player -> cast_pet();
      warlock_t*      o = p -> owner -> cast_warlock();
      warlock_pet_attack_t::player_buff();
      player_spell_power += 0.15 * player -> cast_pet() -> owner -> composite_spell_power( SCHOOL_MAX );
      player_multiplier *= 1.0 + o -> active_dots() * 0.15;
    }
  };

  felhunter_pet_t( sim_t* sim, player_t* owner ) :
      warlock_pet_t( sim, owner, "felhunter", PET_FELHUNTER )
  {
    main_hand_weapon.type       = WEAPON_BEAST;
    main_hand_weapon.min_dmg    = 309.6;
    main_hand_weapon.max_dmg    = 309.6;
    main_hand_weapon.damage     = ( main_hand_weapon.min_dmg + main_hand_weapon.max_dmg ) / 2;
    main_hand_weapon.swing_time = 2.0;

//    damage_modifier = 0.8;

    action_list_str = "shadow_bite/wait_until_ready";
  }
  virtual void init_base()
  {
    warlock_pet_t::init_base();

    base_attack_power = -20;

    resource_base[ RESOURCE_HEALTH ] = 4788;
    resource_base[ RESOURCE_MANA   ] = 1559;

    attribute_base[ ATTR_STRENGTH  ] = 314;
    attribute_base[ ATTR_AGILITY   ] = 90;
    attribute_base[ ATTR_STAMINA   ] = 328;
    attribute_base[ ATTR_INTELLECT ] = 150;
    attribute_base[ ATTR_SPIRIT    ] = 209;

    base_attack_power = -20;
    initial_attack_power_per_strength = 2.0;

    health_per_stamina = 9.5;
    mana_per_intellect = 11.55;
    mp5_per_intellect  = 8.0 / 324.0;

    base_mp5 = 11.22;

    base_attack_crit = 0.0327;
 
    melee = new warlock_pet_melee_t( this, "felhunter_melee" );
  }

  virtual action_t* create_action( const std::string& name,
                                   const std::string& options_str )
  {
    if ( name == "shadow_bite" ) return new shadow_bite_t( this );

    return player_t::create_action( name, options_str );
  }
};

// ==========================================================================
// Pet Succubus
// ==========================================================================

struct succubus_pet_t : public warlock_pet_t
{
  struct lash_of_pain_t : public warlock_pet_spell_t
  {
    lash_of_pain_t( player_t* player ) :
        warlock_pet_spell_t( "lash_of_pain", player, RESOURCE_MANA, SCHOOL_SHADOW )
    {
      static rank_t ranks[] =
      {
        { 80, 9, 237, 237, 0, 250 },
        { 74, 8, 193, 193, 0, 220 },
        { 68, 7, 123, 123, 0, 190 },
        { 60, 6,  99,  99, 0, 160 },
        { 0, 0, 0, 0, 0, 0 }
      };
      init_rank( ranks, 47992 );

      direct_power_mod  = ( 1.5 / 3.5 );
      may_crit          = true;


      cooldown -> duration  = 12.0;
    }
  };

  succubus_pet_t( sim_t* sim, player_t* owner ) :
      warlock_pet_t( sim, owner, "succubus", PET_SUCCUBUS )
  {
    main_hand_weapon.type       = WEAPON_BEAST;
    main_hand_weapon.min_dmg    = 412.5;
    main_hand_weapon.max_dmg    = 412.5;
    main_hand_weapon.damage     = ( main_hand_weapon.min_dmg + main_hand_weapon.max_dmg ) / 2;
    main_hand_weapon.swing_time = 2.0;

    damage_modifier = 1.05;

    action_list_str = "lash_of_pain/wait_until_ready";
  }
  virtual void init_base()
  {
    warlock_pet_t::init_base();

    base_attack_power = -20;

    resource_base[ RESOURCE_HEALTH ] = 1468;
    resource_base[ RESOURCE_MANA   ] = 1559;

    melee = new warlock_pet_melee_t( this, "succubus_melee" );
  }
  virtual action_t* create_action( const std::string& name,
                                   const std::string& options_str )
  {
    if ( name == "lash_of_pain" ) return new lash_of_pain_t( this );

    return player_t::create_action( name, options_str );
  }
};

// ==========================================================================
// Pet Infernal
// ==========================================================================

struct infernal_pet_t : public warlock_pet_t
{
  struct infernal_immolation_t : public warlock_pet_spell_t
  {
    infernal_immolation_t( player_t* player ) :
        warlock_pet_spell_t( "immolation", player, RESOURCE_NONE, SCHOOL_FIRE )
    {
      base_dd_min = base_dd_max = 40;
      direct_power_mod  = 0.2;
      background        = true;
      repeating         = true;
    }

    virtual double execute_time() SC_CONST
    {
      // immolation is an aura that ticks every 2 seconds
      return 2.0;
    }

    virtual void player_buff()
    {
      // immolation uses the master's spell power, not the infernal's
      warlock_pet_t* p = ( warlock_pet_t* ) player -> cast_pet();
      warlock_t* o = p -> owner -> cast_warlock();
      warlock_pet_spell_t::player_buff();
      player_spell_power = o -> composite_spell_power( school );
    }
  };

  infernal_immolation_t* immolation;

  infernal_pet_t( sim_t* sim, player_t* owner ) :
      warlock_pet_t( sim, owner, "infernal", PET_INFERNAL )
  {
    main_hand_weapon.type       = WEAPON_BEAST;
    main_hand_weapon.min_dmg    = 412.5;
    main_hand_weapon.max_dmg    = 412.5;
    main_hand_weapon.damage     = ( main_hand_weapon.min_dmg + main_hand_weapon.max_dmg ) / 2;
    main_hand_weapon.swing_time = 2.0;

    damage_modifier = 3.20;
  }
  virtual void init_base()
  {
    warlock_pet_t::init_base();

    attribute_base[ ATTR_STRENGTH  ] = 331;
    attribute_base[ ATTR_AGILITY   ] = 113;
    attribute_base[ ATTR_STAMINA   ] = 361;
    attribute_base[ ATTR_INTELLECT ] =  65;
    attribute_base[ ATTR_SPIRIT    ] = 109;

    resource_base[ RESOURCE_HEALTH ] = 20300;
    resource_base[ RESOURCE_MANA   ] = 0;

    base_attack_power = -20;

    melee      = new   warlock_pet_melee_t( this, "infernal_melee" );
    immolation = new infernal_immolation_t( this );
  }
  virtual void schedule_ready( double delta_time=0,
                               bool   waiting=false )
  {
    melee -> schedule_execute();
    immolation -> schedule_execute();
  }
  virtual double composite_attack_hit() SC_CONST { return 0; }
  virtual double composite_spell_hit()  SC_CONST { return 0; }
};

// ==========================================================================
// Pet Doomguard
// ==========================================================================

struct doomguard_pet_t : public warlock_pet_t
{
  doomguard_pet_t( sim_t* sim, player_t* owner ) :
      warlock_pet_t( sim, owner, "doomguard", PET_DOOMGUARD )
  {
    main_hand_weapon.type       = WEAPON_BEAST;
    main_hand_weapon.min_dmg    = 412.5;
    main_hand_weapon.max_dmg    = 412.5;
    main_hand_weapon.damage     = ( main_hand_weapon.min_dmg + main_hand_weapon.max_dmg ) / 2;
    main_hand_weapon.swing_time = 2.0;

    damage_modifier = 1.98;
  }

  virtual void init_base()
  {
    warlock_pet_t::init_base();

    resource_base[ RESOURCE_HEALTH ] = 18000;
    resource_base[ RESOURCE_MANA   ] = 3000;

    base_attack_power = -20;

    melee = new warlock_pet_melee_t( this, "doomguard_melee" );
  }
  virtual action_t* create_action( const std::string& name,
                                   const std::string& options_str )
  {
    return player_t::create_action( name, options_str );
  }
};

namespace { // ANONYMOUS NAMESPACE ==========================================

// ==========================================================================
// Warlock Spell
// ==========================================================================

struct warlock_spell_t : public spell_t
{


  warlock_spell_t( const char* n, player_t* player, int s, int t ) :
      spell_t( n, player, RESOURCE_MANA, s, t )
  { }

  // Overridden Methods
  virtual double haste() SC_CONST;
  virtual void   player_buff();
  virtual void   target_debuff( int dmg_type );
  virtual void   execute();
  virtual void   tick();
  virtual void   parse_options( option_t*, const std::string& );
  virtual bool   ready();
};

// trigger_soul_leech =======================================================

static void trigger_soul_leech( spell_t* s )
{
  warlock_t* p = s -> player -> cast_warlock();

  if ( p -> talent_soul_leech -> rank )
  {
    if ( p -> rng_soul_leech -> roll( p -> talent_soul_leech -> rank ) )
    {
      p -> resource_gain( RESOURCE_HEALTH, p -> resource_max[ RESOURCE_HEALTH ] * 0.01, p ->gains_soul_leech );
      p -> resource_gain( RESOURCE_MANA, p -> resource_max[ RESOURCE_MANA ] * 0.01, p -> gains_soul_leech );

      p -> trigger_replenishment();

    }
  }
}


// trigger_decimation ======================================================

static void trigger_decimation( warlock_spell_t* s,
                                int result )
{
  warlock_t* p = s -> player -> cast_warlock();
  if ( ( result !=  RESULT_HIT ) && ( result != RESULT_CRIT ) ) return;
  if ( s -> sim -> target -> health_percentage() > 35  ) return;
  p -> buffs_decimation -> trigger();
}


// trigger_deaths_embrace ===================================================

static int trigger_deaths_embrace( spell_t* s )
{
  warlock_t* p = s -> player -> cast_warlock();

  if ( ! p -> talent_deaths_embrace -> rank ) return 0;

  if ( s -> sim -> target -> health_percentage() < 25 )
  {
    return p -> talent_deaths_embrace -> rank * 4;
  }

  return 0;
}

// trigger_everlasting_affliction ===========================================

static void trigger_everlasting_affliction( spell_t* s )
{
  warlock_t* p = s -> player -> cast_warlock();

  if ( ! p -> talent_everlasting_affliction -> rank ) return;

  if ( ! p -> dots_corruption -> ticking() ) return;

  if ( p -> rng_everlasting_affliction -> roll( util_t::talent_rank( p -> talent_everlasting_affliction -> rank, 3, 0.33, 0.66, 1.00 ) ) )
  {
    p -> dots_corruption -> action -> refresh_duration();
  }
}

// ==========================================================================
// Warlock Spell
// ==========================================================================

// warlock_spell_t::haste ===================================================

double warlock_spell_t::haste() SC_CONST
{
  warlock_t* p = player -> cast_warlock();
  double h = spell_t::haste();

  if ( p -> buffs_eradication -> up() )
  {
    double ranks[] = { 0.0, 0.06, 0.12, 0.20 };
    assert( p -> talent_eradication -> rank <= 3 );
    h *= 1.0 / ( 1.0 + ranks[ p -> talent_eradication -> rank ] );
  }

  if ( tree == TREE_DESTRUCTION && p -> buffs_backdraft -> up() )
  {
    h *= 1.0 - p -> talent_backdraft -> rank * 0.10;
  }

  if ( p -> buffs_improved_soul_fire -> up())
  {
	  h /= ( 1.0 + util_t::talent_rank( p -> talent_improved_soul_fire -> rank, 2, 0.07, 0.15 ) );
  }

  return h;
}

// warlock_spell_t::player_buff =============================================

void warlock_spell_t::player_buff()
{
  warlock_t* p = player -> cast_warlock();

  spell_t::player_buff();
  base_crit_bonus_multiplier = 2;

  if ( p -> buffs_metamorphosis -> up() )
  {
    player_multiplier *= 1.20;
    player_multiplier *= 1.0 + p -> mastery_spells.master_demonologist -> enabled * ( 0.13 + p -> composite_mastery() * 0.015 );

  }

  player_multiplier *= 1.0 + ( p -> talent_demonic_pact -> rank * 0.02 );

  if ( p -> buffs_tier10_4pc_caster -> up() )
  {
    player_multiplier *= 1.10;
  }


  if ( school == SCHOOL_SHADOW )
  {
    player_multiplier *= 1.0 + trigger_deaths_embrace( this ) * 0.01;


  }

}

// warlock_spell_t::target_debuff ============================================

void warlock_spell_t::target_debuff( int dmg_type )
{
  warlock_t* p = player -> cast_warlock();

  spell_t::target_debuff( dmg_type );

  double stone_bonus = 0;

  if ( school == SCHOOL_FIRE )
  {
	  target_multiplier *= 1.0 + ( p -> talent_spec_spells.cataclysm -> enabled * 0.25 );
	  if ( p -> mastery_spells.fiery_apocalypse -> enabled ) target_multiplier *= 1.0 +  p -> mastery_spells.fiery_apocalypse -> enabled * ( 0.11  + p -> composite_mastery() * 0.0125 );
	  target_multiplier *= 1.0 + ( p -> talent_spec_spells.master_demonologist -> enabled * 0.15 );
  }
  if ( school == SCHOOL_SHADOW )
  {
	  target_multiplier *= 1.0 + ( p -> talent_spec_spells.master_demonologist -> enabled * 0.15 );
	  target_multiplier	*= 1.0 + ( p -> talent_spec_spells.shadow_mastery -> enabled * 0.25 );
  }



  if ( dmg_type == DMG_OVER_TIME )
  {
    if ( school == SCHOOL_SHADOW )
    {
      if ( p -> mastery_spells.potent_afflictions -> enabled )
      {
    	  target_multiplier *= 1.0 + ( p -> mastery_spells.potent_afflictions -> enabled * 0.14 ) + (  p -> composite_mastery()  * 0.0163 );
      }
      if ( p -> talent_shadow_embrace -> rank )
      {
        target_multiplier *= 1.0 + p -> buffs_shadow_embrace -> stack() * util_t::talent_rank( p -> talent_shadow_embrace -> rank, 3, 0.03, 0.04, 0.05 );
      }
      if ( p -> buffs_haunted -> up() )
      {
        target_multiplier *= 1.20 + ( p -> glyphs.haunt ? 0.03 : 0.00 );
      }
    }

    if ( p -> main_hand_weapon.buff_type == SPELL_STONE )
    {
      stone_bonus = 0.01;
    }
  }
  else
  {
    if ( p -> main_hand_weapon.buff_type == FIRE_STONE )
    {
      stone_bonus = 0.01;
    }
  }
  if ( stone_bonus > 0 )
  {
      target_multiplier *= 1 + stone_bonus;
  }
}

// warlock_spell_t::execute ==================================================

void warlock_spell_t::execute()
{
  warlock_t* p = player -> cast_warlock();

  if ( time_to_execute > 0 && tree == TREE_DESTRUCTION )
  {
    p -> buffs_backdraft -> decrement();
  }

  spell_t::execute();

}

// warlock_spell_t::tick =====================================================

void warlock_spell_t::tick()
{
  spell_t::tick();
}

// warlock_spell_t::parse_options =============================================

void warlock_spell_t::parse_options( option_t*          options,
                                     const std::string& options_str )
{
  option_t base_options[] =
  {
    { NULL, OPT_UNKNOWN, NULL }
  };
  std::vector<option_t> merged_options;
  spell_t::parse_options( merge_options( merged_options, options, base_options ), options_str );
}

bool warlock_spell_t::ready()
{
	return spell_t::ready();
}
// Curse of Elements Spell ===================================================

struct curse_of_elements_t : public warlock_spell_t
{
  curse_of_elements_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "curse_of_elements", player, SCHOOL_SHADOW, TREE_AFFLICTION )
  {
    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 67, 3, 0, 0, 0, 260 },
      { 56, 2, 0, 0, 0, 200 },
      { 0, 0, 0, 0, 0, 0 }
    };
    init_rank( ranks, 47865 );

  }

  virtual void execute()
  {
    warlock_spell_t::execute();
    if ( result_is_hit() )
    {
      target_t*  t = sim -> target;
      warlock_t* p = player -> cast_warlock();
      t -> debuffs.curse_of_elements -> expire();
      t -> debuffs.curse_of_elements -> trigger( 1, 13 );
      t -> debuffs.curse_of_elements -> player = p;
    }
  }

  virtual bool ready()
  {
    target_t*  t = sim -> target;
    if ( t -> debuffs.curse_of_elements -> check() )
      return false;

    return warlock_spell_t::ready();
  }
};

// Bane of Agony Spell ===========================================================

struct bane_of_agony_t : public warlock_spell_t
{
  bane_of_agony_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "bane_of_agony", player, SCHOOL_SHADOW, TREE_AFFLICTION )
  {
    warlock_t* p = player -> cast_warlock();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 980;
    parse_data( p -> player_data );

    base_crit += p -> talent_doom_and_gloom -> rank * 0.04;

    if ( p -> glyphs.bane_of_agony )
    {
      num_ticks += 2;
      // after patch 3.0.8, the added ticks are double the base damage
      base_td_init = ( base_td_init * 12 + base_td_init * 4 ) / 14;
    }

  }

  virtual void execute()
  {
    warlock_t* p = player -> cast_warlock();
    warlock_spell_t::execute();
    if ( result_is_hit() )
    	{
    	if ( p -> dots_bane_of_doom -> ticking() )
    		p -> dots_bane_of_doom -> action -> cancel();
    	}
  }


};

// Bane of Doom Spell ===========================================================

struct bane_of_doom_t : public warlock_spell_t
{
  bane_of_doom_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "bane_of_doom", player, SCHOOL_SHADOW, TREE_AFFLICTION )
  {
    warlock_t* p = player -> cast_warlock();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 603;
    parse_data( p -> player_data );

    base_crit += p -> talent_doom_and_gloom -> rank * 0.04;
  }

  virtual void execute()
  {
    warlock_t* p = player -> cast_warlock();
    warlock_spell_t::execute();

    if ( result_is_hit() )
    	{
    	if ( p -> dots_bane_of_agony -> ticking() )
    		p -> dots_bane_of_agony -> action -> cancel();
    	}
  }

  virtual void assess_damage( double amount,
                              int    dmg_type )
  {
    warlock_spell_t::assess_damage( amount, DMG_DIRECT );
  }
};

struct burning_embers_t : public warlock_spell_t
{

  burning_embers_t( player_t* player ) :
      warlock_spell_t( "burning_embers", player, SCHOOL_FIRE, TREE_DESTRUCTION )
  {
    num_ticks=7;
    base_tick_time=1.0;
    base_td=1.0; // Fixe ME
    tick_power_mod=1.0;
  }

  virtual void execute()
  {
	tick_may_crit = true;
    warlock_spell_t::execute();
  }

  virtual void tick()
  {
	warlock_spell_t::tick();
  }
};

// Shadow Bolt Spell ===========================================================

struct shadow_bolt_t : public warlock_spell_t
{
  int isb_benefit;
  int isb_trigger;

  shadow_bolt_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "shadow_bolt", player, SCHOOL_SHADOW, TREE_DESTRUCTION ),
      isb_benefit( 0 ), isb_trigger( 0 )
  {
    warlock_t* p = player -> cast_warlock();

    option_t options[] =
    {
      { "isb_benefit",   OPT_BOOL, &isb_benefit   },
      { "isb_trigger",   OPT_BOOL, &isb_trigger   },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 686;
    parse_data( p -> player_data );

    may_crit          = true;

    base_execute_time -= p -> talent_bane -> rank * 0.1;

    base_cost  *= 1.0 - ( p -> glyphs.shadow_bolt * 0.10 );

    base_multiplier *= 1.0 + ( p -> talent_shadow_and_flame -> rank * 0.04 );



  }

  virtual double execute_time() SC_CONST
  {
    warlock_t* p = player -> cast_warlock();
    if ( p -> buffs_shadow_trance -> up() ) return 0;
    return warlock_spell_t::execute_time();
  }

  virtual void schedule_execute()
  {
    warlock_t* p = player -> cast_warlock();
    warlock_spell_t::schedule_execute();
    p -> buffs_shadow_trance -> expire();
  }

  virtual void execute()
  {
    warlock_spell_t::execute();

    if ( result_is_hit() )
    {
      target_t*  t = sim -> target;
      warlock_t* p = player -> cast_warlock();

      p -> buffs_shadow_embrace -> trigger();
      t -> debuffs.improved_shadow_bolt -> trigger( 1, 1.0, p -> talent_shadow_and_flame -> rank / 5.0 );
      trigger_soul_leech( this );
      trigger_everlasting_affliction( this );
    }
  }


  virtual void travel( int    travel_result,
                       double travel_dmg )
  {
    warlock_spell_t::travel( travel_result, travel_dmg );
    trigger_decimation( this, travel_result );
  }

  virtual bool ready()
  {

    if ( ! warlock_spell_t::ready() )
      return false;

    if ( isb_benefit )
      if ( ! sim -> target -> debuffs.improved_shadow_bolt -> check() )
        return false;

    if ( isb_trigger )
      if ( sim -> target -> debuffs.improved_shadow_bolt -> check() )
        return false;

    return true;
  }
};

// Chaos Bolt Spell ===========================================================

struct chaos_bolt_t : public warlock_spell_t
{

  chaos_bolt_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "chaos_bolt", player, SCHOOL_FIRE, TREE_DESTRUCTION )
  {
    warlock_t* p = player -> cast_warlock();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 50796;
    parse_data( p -> player_data );

    may_crit          = true;
    may_resist        = false;
    base_execute_time -= p -> talent_bane -> rank * 0.1;
    cooldown -> duration = 12.0 - ( p -> glyphs.chaos_bolt * 2.0 );
  }

  virtual void execute()
  {
    warlock_spell_t::execute();
    if ( result_is_hit() )
    {
      trigger_soul_leech( this );
    }
  }

  virtual void player_buff()
  {
    warlock_t* p = player -> cast_warlock();
    warlock_spell_t::player_buff();
    if ( p -> dots_immolate -> ticking() )
    {
      player_multiplier *= 1 + 0.02 * p -> talent_fire_and_brimstone -> rank;
    }
  }
};

// Death Coil Spell ===========================================================

struct death_coil_t : public warlock_spell_t
{
  death_coil_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "death_coil", player, SCHOOL_SHADOW, TREE_AFFLICTION )
  {
    warlock_t* p = player -> cast_warlock();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 6789;
    parse_data( p -> player_data );

    may_crit          = true;
    binary            = true;
  }

  virtual void execute()
  {
    warlock_spell_t::execute();
    if ( result_is_hit() )
    {
      player -> resource_gain( RESOURCE_HEALTH, direct_dmg );
    }
  }

};

// Shadow Burn Spell ===========================================================

struct shadowburn_t : public warlock_spell_t
{
  shadowburn_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "shadowburn", player, SCHOOL_SHADOW, TREE_DESTRUCTION )
  {
    warlock_t* p = player -> cast_warlock();
    check_talent( p -> talent_shadowburn -> rank );

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 17877;
    parse_data( p -> player_data );

    may_crit = true;
  }

  virtual void execute()
  {
    warlock_spell_t::execute();
    if ( result_is_hit() )
    {
      trigger_soul_leech( this );
    }
  }

  virtual void player_buff()
  {
    warlock_t* p = player -> cast_warlock();
    warlock_spell_t::player_buff();
    if ( p -> glyphs.shadow_burn )
    {
      if ( sim -> target -> health_percentage() < 35 )
      {
        player_crit += 0.20;
      }
    }
  }
};

// Shadowfury Spell ===========================================================

struct shadowfury_t : public warlock_spell_t
{
  double cast_gcd;

  shadowfury_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "shadowfury", player, SCHOOL_FIRE, TREE_DESTRUCTION ), cast_gcd( -1 )
  {
    warlock_t* p = player -> cast_warlock();
    check_talent( p -> talent_shadowfury -> rank );

    option_t options[] =
    {
      { "cast_gcd",    OPT_FLT,  &cast_gcd    },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 30283;
    parse_data( p -> player_data );

    may_crit = true;

    // estimate - measured at ~0.6sec, but lag in there too, plus you need to mouse-click
    trigger_gcd = ( cast_gcd >= 0 ) ? cast_gcd : 0.5;

  }
};


// Corruption Spell ===========================================================

struct corruption_t : public warlock_spell_t
{
 corruption_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "corruption", player, SCHOOL_SHADOW, TREE_AFFLICTION )
  {
    warlock_t* p = player -> cast_warlock();

    option_t options[] =
    {
     { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 172;
    parse_data ( p -> player_data);

    base_crit += p -> talent_everlasting_affliction -> rank * 0.05;
    base_multiplier *= 1.0 + ( p -> talent_improved_corruption -> rank  * 0.04 );
    tick_may_crit = true;
  }

  virtual void execute()
  {
    warlock_spell_t::execute();
  }

  virtual void tick()
  {
    warlock_t* p = player -> cast_warlock();
    warlock_spell_t::tick();
    p -> buffs_eradication -> trigger();
    p -> buffs_shadow_trance -> trigger( 1, 1.0, p -> talent_nightfall -> rank * 0.02 );
    p -> buffs_shadow_trance -> trigger( 1, 1.0, p -> glyphs.corruption * 0.04 );
    p -> buffs_molten_core -> trigger(3);
    p -> resource_gain( RESOURCE_HEALTH, tick_dmg * util_t::talent_rank( p -> talent_siphon_life -> rank, 23, 0.4, 0.6 ) );
  }

};

// Drain Life Spell ===========================================================

struct drain_life_t : public warlock_spell_t
{
  drain_life_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "drain_life", player, SCHOOL_SHADOW, TREE_AFFLICTION )
  {
	warlock_t* p = player -> cast_warlock();
    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 689;
    parse_data ( p -> player_data);

    channeled         = true;
    binary            = true;

  }

  virtual void execute()
  {
    warlock_spell_t::execute();
    if ( result_is_hit() )
    {
      trigger_everlasting_affliction( this );
    }
  }

  virtual void player_buff()
  {
    warlock_t* p = player -> cast_warlock();

    warlock_spell_t::player_buff();

    double min_multiplier[] = { 0, 0.03, 0.06 };
    double max_multiplier[] = { 0, 0.09, 0.18 };

    assert( p -> talent_soul_siphon -> rank >= 0 &&
            p -> talent_soul_siphon -> rank <= 2 );

    double min = min_multiplier[ p -> talent_soul_siphon -> rank ];
    double max = max_multiplier[ p -> talent_soul_siphon -> rank ];

    double multiplier = p -> affliction_effects() * min;

    if ( multiplier > max ) multiplier = max;

    player_multiplier *= 1.0 + multiplier;
  }

  virtual void tick()
  {
    warlock_t* p = player -> cast_warlock();
    warlock_spell_t::tick();
    p -> buffs_shadow_trance -> trigger( 1, 1.0, p -> talent_nightfall -> rank * 0.02 );
  }
};

// Drain Soul Spell ===========================================================

struct drain_soul_t : public warlock_spell_t
{
  int interrupt;

  drain_soul_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "drain_soul", player, SCHOOL_SHADOW, TREE_AFFLICTION ), interrupt( 0 )
  {
    warlock_t* p = player -> cast_warlock();

    option_t options[] =
    {
      { "interrupt",  OPT_BOOL, &interrupt },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 1120;
    parse_data( p -> player_data);

    base_dd_min=base_td;
    base_dd_max=base_td;
    channeled         = true;
    binary            = true;

  }

  virtual void execute()
  {
    warlock_spell_t::execute();
    if ( result_is_hit() )
    {
      warlock_t* p = player -> cast_warlock();
      trigger_everlasting_affliction( this );
      if ( p -> rng_pandemic -> roll( p -> talent_pandemic -> rank * 0.5 ) && (sim -> target -> health_percentage() < 25 ) )
      {
          if ( p -> dots_unstable_affliction -> ticking() )
          {

              p -> dots_unstable_affliction -> action -> refresh_duration();

          }
          if ( p -> dots_bane_of_agony -> ticking() )
          {

              p -> dots_bane_of_agony -> action -> refresh_duration();

          }
      }
    }
  }

  virtual void tick()
  {
    warlock_spell_t::tick();

    if ( interrupt && ( current_tick != num_ticks ) )
    {
      warlock_t* p = player -> cast_warlock();
      // If any spell ahead of DS in the action list is "ready", then cancel the DS channel
      for ( action_t* action = p -> action_list; action != this; action = action -> next )
      {
        if ( action -> background ) continue;
        if ( action -> ready() )
        {
          current_tick = num_ticks;
          break;
        }
      }
    }
  }

  virtual void player_buff()
  {
    warlock_spell_t::player_buff();

    warlock_t* p = player -> cast_warlock();

    double min_multiplier[] = { 0, 0.03, 0.06 };
    double max_multiplier[] = { 0, 0.09, 0.18 };

    assert( p -> talent_soul_siphon -> rank >= 0 &&
            p -> talent_soul_siphon -> rank <= 2 );

    double min = min_multiplier[ p -> talent_soul_siphon -> rank ];
    double max = max_multiplier[ p -> talent_soul_siphon -> rank ];

    double multiplier = p -> affliction_effects() * min;

    if ( multiplier > max ) multiplier = max;

    player_multiplier *= 1.0 + multiplier;

    // FIXME! Hack! Deaths Embrace is additive with Drain Soul "execute".
    // Perhaps it is time to add notion of "execute" into action_t class.

    int de_bonus = trigger_deaths_embrace( this );
    if ( de_bonus ) player_multiplier /= 1.0 + de_bonus * 0.01;


      if ( sim -> target -> health_percentage() < 25 )
      {
        player_multiplier *= 4.0 + de_bonus * 0.01;
      }

  }

};

// Unstable Affliction Spell ======================================================

struct unstable_affliction_t : public warlock_spell_t
{
  unstable_affliction_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "unstable_affliction", player, SCHOOL_SHADOW, TREE_AFFLICTION )
  {
    warlock_t* p = player -> cast_warlock();
    check_talent( p -> talent_spec_spells.unstable_affliction -> enabled );

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 30108;
    parse_data ( p -> player_data);

    base_crit += p -> talent_everlasting_affliction -> rank * 0.05;
    tick_may_crit = true;

    if ( p -> glyphs.unstable_affliction )
    {
      base_execute_time = 1.3;
      //trigger_gcd     = 1.3; latest research seems to imply it does not affect the gcd
    }

  }
  virtual void tick()
  {
    warlock_spell_t::tick();

  }
};

// Haunt Spell ==============================================================

struct haunt_t : public warlock_spell_t
{
  int debuff;

  haunt_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "haunt", player, SCHOOL_SHADOW, TREE_AFFLICTION )
  {
    warlock_t* p = player -> cast_warlock();
    check_talent( p -> talent_haunt -> rank );

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };

    parse_options( options, options_str );

    id = 48181;
    parse_data ( p -> player_data);
    direct_power_mod=1.5/3.5; // hack for now, no data avaiable
    may_crit = true;

  }

  virtual void execute()
  {
    warlock_spell_t::execute();
    if ( result_is_hit() )
    {
      warlock_t* p = player -> cast_warlock();
      p -> buffs_haunted -> trigger();
      p -> buffs_shadow_embrace -> trigger();
      trigger_everlasting_affliction( this );
    }
  }

};
// Immolate Burst Spell ======================================================

struct immolate_burst_t : public warlock_spell_t
{
	  immolate_burst_t( player_t* player ) :
	      warlock_spell_t( "immolate", player, SCHOOL_FIRE, TREE_DESTRUCTION )

	  {
		  warlock_t* p = player -> cast_warlock();

	    proc       = true;
	    background = true;
	    may_crit   = true;

	    id = 348;
	    effect_nr=2;
	    parse_effect_data( p -> player_data);

	    base_multiplier = 1.0 + ( p -> talent_improved_immolate -> rank * 0.10 );

	    name_str = "immolate_burst";
	  }
	  virtual void execute()
	  {
	    warlock_spell_t::execute();
	    update_stats( DMG_DIRECT );
	  }
};

// Immolate Spell =============================================================

struct immolate_t : public warlock_spell_t
{
  cooldown_t* conflagrate_cooldown;
  double      conflagrate_lag;
  spell_t*	immolate_burst;

  immolate_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "immolate", player, SCHOOL_FIRE, TREE_DESTRUCTION ),
      conflagrate_cooldown(0), conflagrate_lag(0.5), immolate_burst( 0 )
  {
    warlock_t* p = player -> cast_warlock();

    option_t options[] =
    {
      { "conflagrate_lag", OPT_FLT, &conflagrate_lag },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 348;
    parse_data ( p -> player_data);


    may_crit      = true;
    tick_may_crit = true;
    base_multiplier = 1.0 + ( p -> talent_improved_immolate -> rank * 0.10 );
    base_execute_time -= p -> talent_bane -> rank * 0.1;
    base_td_multiplier *= 1.0 + ( p -> glyphs.immolate              * 0.10 );
    conflagrate_cooldown = p -> get_cooldown( "conflagrate" );

    immolate_burst = new immolate_burst_t( p );
    tick_power_mod = immolate_burst -> direct_power_mod;

  }

  virtual void execute()
  {
    warlock_spell_t::execute();
    if ( conflagrate_lag > 0 )
    {
      double travel_ready = sim -> current_time + conflagrate_lag;
      if ( travel_ready > conflagrate_cooldown -> ready )
      {
    	  conflagrate_cooldown -> ready = travel_ready;
      }
    }
    if ( immolate_burst )
    {
      immolate_burst -> execute();
    }
  }


};

// Shadowflame Spell =============================================================

struct shadowflame_t : public warlock_spell_t
{
  shadowflame_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "shadowflame", player, SCHOOL_SHADOW, TREE_DESTRUCTION )
  {
	warlock_t* p = player -> cast_warlock();
	option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 47897;
    parse_data ( p -> player_data);

    may_crit          = true;

  }

  virtual void execute()
  {
    base_td = base_td_init;
    // DD is shadow damage, DoT is fire damage
    school = SCHOOL_SHADOW;
    warlock_spell_t::execute();
    if ( result_is_hit() )
    {
      // DD was shadow, now DoT is fire, so reset school
      school = SCHOOL_FIRE;
    }
  }
};

// Conflagrate Spell =========================================================

struct conflagrate_t : public warlock_spell_t
{
  int ticks_lost;

  action_t* dot_spell;
  double immolate_multiplier;
  double shadowflame_multiplier;

  conflagrate_t( player_t* player, const std::string& options_str ) :
    warlock_spell_t( "conflagrate", player, SCHOOL_FIRE, TREE_DESTRUCTION ), 
    ticks_lost( 0 ), dot_spell( 0 ), immolate_multiplier( 1.0 ), shadowflame_multiplier( 1.0 )
  {
    warlock_t* p = player -> cast_warlock();
    check_talent( p -> talent_spec_spells.conflagrate -> enabled );

    option_t options[] =
    {
      { "ticks_lost", OPT_INT, &ticks_lost },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    static rank_t ranks[] =
    {
      { 40, 1, 0, 0, 0, 0.16 },
      {  0, 0, 0, 0, 0, 0    }
    };
    init_rank( ranks, 17962 );

    may_crit = true;
    base_execute_time = 0;
    direct_power_mod  = ( 1.5/3.5 );

    cooldown -> duration = 10;


    base_crit += p -> talent_fire_and_brimstone -> rank * 0.05 ;

    immolate_multiplier *= 1.0 + ( p -> glyphs.immolate              * 0.10 );


    base_tick_time = 1.0;
    num_ticks      = 3;
    tick_may_crit  = true;
  }

  virtual double calculate_tick_damage()
  {
    warlock_spell_t::calculate_tick_damage();
    warlock_t* p = player -> cast_warlock();

    if ( p -> dots_immolate -> ticking() )
    {
      tick_dmg *= immolate_multiplier;
    }
    else
    {
      tick_dmg *= shadowflame_multiplier;
    }

    tick_dmg /= 3;

    // 3.3.2 buff
    tick_dmg *= 2.0;

    return tick_dmg;
  }

  virtual double calculate_direct_damage()
  {
    warlock_spell_t::calculate_direct_damage();
    warlock_t* p = player -> cast_warlock();
    if ( p -> dots_immolate -> ticking() )
    {
      direct_dmg = 1;
    }
    return direct_dmg;
  }

  virtual double total_spell_power() SC_CONST
  {
    return dot_spell -> total_spell_power();
  }

  virtual void execute()
  {
    warlock_t* p = player -> cast_warlock();

    if      (   p -> dots_immolate -> ticking() && ! p -> dots_shadowflame -> ticking() ) dot_spell = p -> dots_immolate -> action;
    else if ( ! p -> dots_immolate -> ticking() &&   p -> dots_shadowflame -> ticking() ) dot_spell = p -> dots_shadowflame -> action;
    else if ( sim -> rng -> roll( 0.50 ) )                          dot_spell = p -> dots_immolate -> action;
    else                                                            dot_spell = p -> dots_shadowflame -> action;

    int tick_contribution = 0;
    if( dot_spell == p -> dots_immolate -> action )
    {
      tick_contribution = 3;
    }
    else
    {
      tick_contribution = 4;
    }

    base_dd_min      = dot_spell -> base_td   * tick_contribution;
    base_dd_max      = dot_spell -> base_td   * tick_contribution;
    direct_power_mod = dot_spell -> tick_power_mod * tick_contribution;

    base_td        = dot_spell -> base_td;
    tick_power_mod = dot_spell -> tick_power_mod;

    warlock_spell_t::execute();

    if ( result_is_hit() )
    {
      trigger_soul_leech( this );
      p -> buffs_backdraft -> trigger( 3 );
      if ( ! p -> glyphs.conflagrate )
      {
        dot_spell -> cancel();
      }
    }
  }

  virtual bool ready()
  {
    warlock_t* p = player -> cast_warlock();

    // If there is neither an active immolate nor shadowflame, then conflag is not ready
    if ( ! ( p -> dots_immolate -> ticking() || p -> dots_shadowflame -> ticking() ) ) return false;

    if ( ticks_lost > 0 )
    {
      // The "ticks_lost" checking is a human decision and should not have any RNG.
      // The priority will always be to preserve the Immolate spell if both are up.
      int ticks_remaining = 0;

      if ( p -> dots_immolate -> ticking() )
      {
        ticks_remaining = ( p -> dots_immolate -> action -> num_ticks -
							p -> dots_immolate -> action -> current_tick );
      }
      else
      {
        ticks_remaining = ( p -> dots_shadowflame -> action -> num_ticks -
                            p -> dots_shadowflame -> action -> current_tick );
      }

      if ( ticks_remaining > ticks_lost )
        return false;
    }

    return warlock_spell_t::ready();
  }
};

// Incinerate Burst Immolate Spell ======================================================

struct incinerate_burst_immolate_t : public warlock_spell_t
{
	incinerate_burst_immolate_t( player_t* player ) :
	      warlock_spell_t( "incinerate", player, SCHOOL_FIRE, TREE_DESTRUCTION )

	  {
		  warlock_t* p = player -> cast_warlock();

	    proc       = true;
	    background = true;
	    may_crit   = true;

	    id = 29722;
	    parse_effect_data( p -> player_data);

	    name_str = "incinerate_burst_immolate";
	  }
	  virtual void execute()
	  {
	    warlock_spell_t::execute();
	    update_stats( DMG_DIRECT );
	  }
};

// Incinerate Spell =========================================================

struct incinerate_t : public warlock_spell_t
{
  spell_t*	incinerate_burst_immolate;

  incinerate_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "incinerate", player, SCHOOL_FIRE, TREE_DESTRUCTION ),
      incinerate_burst_immolate( 0 )
  {
    warlock_t* p = player -> cast_warlock();

    travel_speed = 21.0; // set before options to allow override

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 29722;
    parse_data ( p -> player_data);

    may_crit           = true;
    base_multiplier *= 1.0 + ( p -> talent_shadow_and_flame -> rank * 0.04 );
    base_execute_time -= util_t::talent_rank( p -> talent_emberstorm -> rank, 2, 0.13, 0.25 );
    base_multiplier *= 1.0 + ( p -> glyphs.incinerate            * 0.05 );

    incinerate_burst_immolate = new incinerate_burst_immolate_t( p );
  }

  virtual void execute()
  {
    warlock_spell_t::execute();
    if ( incinerate_burst_immolate )
       {
    	incinerate_burst_immolate -> execute();
       }
  }

  virtual void schedule_travel()
  {
    warlock_spell_t::schedule_travel();
  }

  virtual void travel( int    travel_result,
                       double travel_dmg )
  {
    warlock_spell_t::travel( travel_result, travel_dmg );
    trigger_decimation( this, travel_result );
  }

  virtual void player_buff()
  {
    warlock_t* p = player -> cast_warlock();
    warlock_spell_t::player_buff();
    if ( p -> buffs_molten_core -> up() ) {
      player_multiplier *= 1 + p -> talent_molten_core -> rank * 0.06;
      p -> buffs_molten_core -> decrement();
    }
    if ( p -> dots_immolate -> ticking() )
    {
      player_multiplier *= 1 + 0.02 * p -> talent_fire_and_brimstone -> rank;
    }
  }

  double haste() SC_CONST
  {
    warlock_t* p = player -> cast_warlock();
    double h = warlock_spell_t::haste();
    if ( p -> buffs_molten_core -> up() )
    {
      h *= 1.0 - p -> talent_molten_core -> rank * 0.10;
    }
    return h;
  }

};

// Searing Pain Spell =========================================================

struct searing_pain_t : public warlock_spell_t
{
  searing_pain_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "searing_pain", player, SCHOOL_FIRE, TREE_DESTRUCTION )
  {
    warlock_t* p = player -> cast_warlock();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 5676;
    parse_data ( p -> player_data);

    may_crit          = true;

    base_crit_bonus_multiplier *= 1.0 + ( p -> glyphs.searing_pain * 0.20 );
  }

  virtual void execute()
  {
	warlock_t* p = player -> cast_warlock();
	if ( sim -> target -> health_percentage() <= 50 && p -> talent_improved_searing_pain -> rank )
		{
			base_crit += p -> talent_improved_searing_pain -> rank * 0.20;
		}
    warlock_spell_t::execute();

  }
};

// Soul Fire Spell ============================================================

struct soul_fire_t : public warlock_spell_t
{
  soul_fire_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "soul_fire", player, SCHOOL_FIRE, TREE_DESTRUCTION )
  {
    warlock_t* p = player -> cast_warlock();

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 6353;
    parse_data ( p -> player_data);

    may_crit          = true;

    base_execute_time -= p -> talent_emberstorm -> rank * 0.5;

  }

  virtual void execute()
  {
    warlock_spell_t::execute();
    if ( result_is_hit() )
    {
      warlock_t* p = player -> cast_warlock();
      trigger_soul_leech( this );
      if ( p -> buffs_empowered_imp -> up() ) p -> buffs_empowered_imp -> expire();
      if ( p -> spells_burning_embers)
    	  {
			  // Trigger Burning Embers
		  }
      if ( ( sim -> target -> health_percentage() >= 80 ) && ( p -> cooldowns_improved_soul_fire -> remains() <=0 ) )
		  {
			  p -> buffs_improved_soul_fire -> trigger();
			  p -> cooldowns_improved_soul_fire -> start();
		  }
    }
  }

  virtual double execute_time() SC_CONST
  {
    warlock_t* p = player -> cast_warlock();
    double t = warlock_spell_t::execute_time();
    if ( p -> buffs_decimation -> up() )
    {
      t *= 1.0 - p -> talent_decimation -> rank * 0.20;
      assert( t > 0 );
    }
    if ( p -> buffs_empowered_imp -> up() )
    {
    	t = 0;
    }
    return t;
  }


  virtual void schedule_travel()
  {
    warlock_spell_t::schedule_travel();

  }

  virtual void travel( int    travel_result,
                       double travel_dmg )
  {
    warlock_spell_t::travel( travel_result, travel_dmg );

    trigger_decimation( this, travel_result );
  }

  virtual void player_buff()
  {
    warlock_t* p = player -> cast_warlock();
    warlock_spell_t::player_buff();
    if ( p -> buffs_molten_core -> up() )
    {
      player_crit += p -> talent_molten_core -> rank * 0.05;
      player_multiplier *= 1 + p -> talent_molten_core -> rank * 0.06;
      p -> buffs_molten_core -> decrement();
    }
  }
};

// Life Tap Spell ===========================================================

struct life_tap_t : public warlock_spell_t
{
  int    buff_refresh;
  double trigger;
  double max_mana_pct;

  life_tap_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "life_tap", player, SCHOOL_SHADOW, TREE_AFFLICTION ),
      buff_refresh(0), trigger(0), max_mana_pct(0)
  {
    id = 57946;

    option_t options[] =
    {
      { "buff_refresh",     OPT_BOOL, &buff_refresh     },
      { "mana_percentage<", OPT_FLT,  &max_mana_pct     },
      { "trigger",          OPT_FLT,  &trigger          },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    harmful = false;

  }

  virtual void execute()
  {
    warlock_t* p = player -> cast_warlock();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );

    double life = p -> resource_max[ RESOURCE_HEALTH ] * 0.2;
    p -> resource_loss( RESOURCE_HEALTH, life );
    p -> resource_gain( RESOURCE_MANA, life * 0.75, p -> gains_life_tap );
    if ( p -> talent_mana_feed -> rank ) p -> active_pet -> resource_gain( RESOURCE_MANA, life * 0.75 * 0.6 * p -> talent_mana_feed -> rank );
    p -> buffs_life_tap_glyph -> trigger();
  }

  virtual bool ready()
  {
    warlock_t* p = player -> cast_warlock();

    if (  max_mana_pct > 0 ) 
      if( ( 100.0 * p -> resource_current[ RESOURCE_MANA ] / p -> resource_max[ RESOURCE_MANA ] ) > max_mana_pct )
        return false;

    if ( buff_refresh )
    {
      if( ! p -> glyphs.life_tap )
        return false;

      if ( p -> buffs_life_tap_glyph -> check() )
        return false;
    }


    if ( trigger > 0 )
      if ( p -> resource_current[ RESOURCE_MANA ] > trigger )
        return false;

    return warlock_spell_t::ready();
  }
};

// Fel Armor Spell ==========================================================

struct fel_armor_t : public warlock_spell_t
{
  double bonus_spell_power;

  fel_armor_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "fel_armor", player, SCHOOL_SHADOW, TREE_DEMONOLOGY ), bonus_spell_power( 0 )
  {
    warlock_t* p = player -> cast_warlock();

    id = 28176;

    harmful = false;
    trigger_gcd = 0;
    bonus_spell_power = p -> player_data.effect_base_value ( p -> player_data.spell_effect_id( id, 1 ) );

    // Model the passive health tick.....
    base_tick_time = 5.0;
    num_ticks      = 1;

    id = 47893;
  }

  virtual void execute()
  {
    warlock_t* p = player -> cast_warlock();

    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );

    p -> buffs_fel_armor -> start( 1, bonus_spell_power );

    schedule_tick();
  }

  virtual void tick()
  {
    warlock_t* p = player -> cast_warlock();
    current_tick = 0; // ticks indefinitely
    p -> resource_gain( RESOURCE_HEALTH, p -> resource_max[ RESOURCE_HEALTH ] * 0.02 * ( 1.0 + p -> talent_demonic_aegis -> rank * 0.5 ), p -> gains_fel_armor, this );
  }

  virtual bool ready()
  {
    warlock_t* p = player -> cast_warlock();
    return ! p -> buffs_fel_armor -> check();
  }
};

// Summon Pet Spell ==========================================================

struct summon_pet_t : public warlock_spell_t
{
  std::string pet_name;

  summon_pet_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "summon_pet", player, SCHOOL_SHADOW, TREE_DEMONOLOGY )
  {
    warlock_t* p = player -> cast_warlock();
    pet_name = ( options_str.size() > 0 ) ? options_str : p -> summon_pet_str;
    harmful = false;
    trigger_gcd = 0;
  }

  virtual void execute()
  {
    player -> summon_pet( pet_name.c_str() );
  }

  virtual bool ready()
  {
    warlock_t* p = player -> cast_warlock();
    if ( p -> active_pet ) return false;
    if ( sim -> current_time > 10.0 ) return false;
    return true;
  }
};


// Immolation Spell =======================================================

struct immolation_t : public warlock_spell_t
{
  immolation_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "immolation", player, SCHOOL_FIRE, TREE_DEMONOLOGY )
  {
	warlock_t* p = player -> cast_warlock();
    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 50589;
    parse_data ( p -> player_data);
  }

  virtual double tick_time() SC_CONST
  {
    return base_tick_time * haste();
  }

  virtual void tick()
  {
    warlock_t* p = player -> cast_warlock();
    if ( p -> buffs_metamorphosis -> check() )
      warlock_spell_t::tick();
    else
      current_tick = num_ticks;
  }

  virtual bool ready()
  {
	  warlock_t* p = player -> cast_warlock();
	  if (p -> buffs_metamorphosis -> check() ) return true;
	  return false;
  }
};

// Metamorphosis Spell =======================================================

struct metamorphosis_t : public warlock_spell_t
{
  metamorphosis_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "metamorphosis", player, SCHOOL_SHADOW, TREE_DEMONOLOGY )
  {
    warlock_t* p = player -> cast_warlock();
    check_talent( p -> talent_metamorphosis -> rank );

    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    harmful = false;
    base_cost   = 0;
    trigger_gcd = 0;

    cooldown -> duration = 180 * ( 1.0 - p -> talent_nemesis -> rank * 0.15 );

    id = 59672;
  }

  virtual void execute()
  {
    warlock_t* p = player -> cast_warlock();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );
    update_ready();
    p -> buffs_metamorphosis -> start();
  }
};

// Demonic Empowerment Spell ================================================

struct demonic_empowerment_t : public warlock_spell_t
{
  int demonic_frenzy;

  demonic_empowerment_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "demonic_empowerment", player, SCHOOL_SHADOW, TREE_DEMONOLOGY ), demonic_frenzy( 0 )
  {
    warlock_t* p = player -> cast_warlock();
    check_talent( p -> talent_demonic_empowerment -> rank );

    option_t options[] =
    {
      { "demonic_frenzy", OPT_INT, &demonic_frenzy },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    harmful = false;
    base_cost = player -> resource_base[ RESOURCE_MANA ] * 0.06;
    cooldown -> duration  = 60 * ( 1.0 - p -> talent_nemesis -> rank * 0.15 );
    trigger_gcd = 0;

    id = 47193;
  }

  virtual void execute()
  {
    warlock_t* p = player -> cast_warlock();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );
    update_ready();
    if( p -> active_pet -> pet_type == PET_FELGUARD )
    {
      p -> buffs_demonic_empowerment -> duration = 15.0;
      p -> buffs_demonic_empowerment -> trigger();
    }
    else if( p -> active_pet -> pet_type == PET_IMP )
    {
      p -> buffs_demonic_empowerment -> duration = 30.0;
      p -> buffs_demonic_empowerment -> trigger();
    }
    else assert( false );
  }

  virtual bool ready()
  {
    warlock_t* p = player -> cast_warlock();

    if ( ! p -> active_pet )
      return false;

    if( p -> active_pet -> pet_type == PET_FELGUARD )
    {
      if ( demonic_frenzy )
        if ( p -> buffs_demonic_frenzy -> current_stack < demonic_frenzy ) 
          return false;
    }
    else if( p -> active_pet -> pet_type == PET_IMP )
    {
    }
    else return false;

    return warlock_spell_t::ready();
  }
};

// Fire Stone Spell ===========================================================

struct fire_stone_t : public warlock_spell_t
{
  int bonus_crit;

  fire_stone_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "fire_stone", player, SCHOOL_FIRE, TREE_DEMONOLOGY )
  {
    warlock_t* p = player -> cast_warlock();
    trigger_gcd = 0;

    harmful = false;

    bonus_crit = ( int ) util_t::ability_rank( p -> level,  49,80,  42,74,  35,66,  28,0 );


    id = 55158;
  }

  virtual void execute()
  {
    if ( sim -> log ) log_t::output( sim, "%s performs %s", player -> name(), name() );

    player -> main_hand_weapon.buff_type = FIRE_STONE;
    player -> spell_crit += bonus_crit / player -> rating.spell_crit;
  };

  virtual bool ready()
  {
    return( player -> main_hand_weapon.buff_type != FIRE_STONE );
  }
};

// Spell Stone Spell ===========================================================

struct spell_stone_t : public warlock_spell_t
{
  int bonus_haste;

  spell_stone_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "spell_stone", player, SCHOOL_FIRE, TREE_DEMONOLOGY )
  {
    warlock_t* p = player -> cast_warlock();
    trigger_gcd = 0;

    harmful = false;

    bonus_haste = util_t::ability_rank( p -> level,  60,80,  50,72,  40,66,  30,0 );


    id = 55194;
  }

  virtual void execute()
  {
    if ( sim -> log ) log_t::output( sim, "%s performs %s", player -> name(), name() );

    player -> main_hand_weapon.buff_type = SPELL_STONE;
    player -> haste_rating += bonus_haste;
    player -> recalculate_haste();
  };

  virtual bool ready()
  {
    return( player -> main_hand_weapon.buff_type != SPELL_STONE );
  }
};



// Hand of Gul'dan Spell =============================================================

struct hand_of_guldan_t : public warlock_spell_t
{

  hand_of_guldan_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "hand_of_guldan", player, SCHOOL_FIRE, TREE_DESTRUCTION )
  {
    warlock_t* p = player -> cast_warlock();
    check_talent( p -> talent_hand_of_guldan -> rank );
    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 71521;
    parse_data ( p -> player_data);


  }

  virtual void execute()
  {
	warlock_t* p = player -> cast_warlock();
    warlock_spell_t::execute();
    p -> buffs_hand_of_guldan -> trigger();
    if ( p -> dots_immolate -> ticking() )
    	{
			if ( p -> rng_cremation -> roll( p -> talent_cremation -> rank * 0.5 ) )
			{
				p -> dots_immolate -> action -> refresh_duration();
			}
    	}
  }


};

// Fel Flame Spell =========================================================

struct fel_flame_t : public warlock_spell_t
{

	fel_flame_t( player_t* player, const std::string& options_str ) :
      warlock_spell_t( "fel_flame", player, SCHOOL_FIRE, TREE_DESTRUCTION )
  {
    warlock_t* p = player -> cast_warlock();


    option_t options[] =
    {
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    id = 29722;
    parse_data ( p -> player_data);

    may_crit           = true;

  }

  virtual void execute()
  {
    warlock_spell_t::execute();
    if ( result_is_hit() )
    {
      warlock_t* p = player -> cast_warlock();
      if ( p -> dots_immolate -> ticking() )p -> dots_immolate -> action -> extend_duration( 2 );
      if ( p -> dots_unstable_affliction -> ticking() )p -> dots_unstable_affliction -> action -> extend_duration( 2 );
    }
  }

};

struct dark_intent_t : public warlock_spell_t
{
  player_t*          dark_intent_target;
  action_callback_t* dark_intent_cb;

  dark_intent_t( player_t* player, const std::string& options_str ) :
    warlock_spell_t( "dark_intent", player, SCHOOL_SHADOW, TREE_AFFLICTION ),
    dark_intent_target(0), dark_intent_cb(0)
  {
    warlock_t* p = player -> cast_warlock();

    std::string target_str = p -> dark_intent_target_str;
    option_t options[] =
    {
      { "target", OPT_STRING, &target_str },
      { NULL, OPT_UNKNOWN, NULL }
    };
    parse_options( options, options_str );

    if ( target_str.empty() )
    {
      // If no target specified, assume 100% up-time by forcing "buffs.dark_intent_feedback = 1"
    	dark_intent_target = p;
    }
    else
    {
    	dark_intent_target = sim -> find_player( target_str );

      assert ( dark_intent_target != 0 );
      assert ( dark_intent_target != p );
    }

    trigger_gcd = 0;

    struct dark_intent_feedback_callback_t : public action_callback_t
    {
    	dark_intent_feedback_callback_t( player_t* p ) : action_callback_t( p -> sim, p ) {}

      virtual void trigger( action_t* a )
      {
        listener -> cast_warlock() -> buffs_dark_intent_feedback -> trigger();
      }
    };

    dark_intent_cb = new dark_intent_feedback_callback_t( p );
    dark_intent_cb -> active = false;
    dark_intent_target -> register_spell_result_callback( RESULT_CRIT_MASK, dark_intent_cb );

    id = 54646;
  }

  virtual void execute()
  {
    warlock_t* p = player -> cast_warlock();
    if ( sim -> log ) log_t::output( sim, "%s performs %s", p -> name(), name() );
    if ( dark_intent_target == p )
    {
      if ( sim -> log ) log_t::output( sim, "%s grants SomebodySomewhere Dark Intent", p -> name() );
      p -> buffs_dark_intent_feedback -> override();
    }
    else
    {
      if ( sim -> log ) log_t::output( sim, "%s grants %s Dark Intent", p -> name(), dark_intent_target -> name() );
      dark_intent_target -> buffs.dark_intent -> trigger();
      dark_intent_cb -> active = true;
      warlock_t* p = player -> cast_warlock();
      p -> buffs.dark_intent -> trigger();

    }
  }

  virtual bool ready()
  {
    warlock_t* p = player -> cast_warlock();

    if ( dark_intent_target == p )
    {
      return ! p -> buffs_dark_intent_feedback -> check();
    }
    else
    {
      return ! dark_intent_target -> buffs.dark_intent -> check();
    }
  }
};


} // ANONYMOUS NAMESPACE ====================================================

// ==========================================================================
// Warlock Pet Events
// ==========================================================================

// demonic_pact_callback ===================================================

struct demonic_pact_callback_t : public action_callback_t
{
  double cooldown_ready;

  demonic_pact_callback_t( player_t* p ) : action_callback_t( p -> sim, p ), cooldown_ready(0) {}

  virtual void reset() { action_callback_t::reset(); cooldown_ready=0; }

  virtual void trigger( action_t* a )
  {
    if ( sim -> current_time < cooldown_ready ) return;
    cooldown_ready = sim -> current_time + 20;

    warlock_t* o = listener -> cast_pet() -> owner -> cast_warlock();

    // HACK ALERT!!!  To prevent spell power contributions from ToW/FT/IDS/DP buffs, we fiddle with player type
    o -> type = PLAYER_GUARDIAN;
    double buff = o -> composite_spell_power( SCHOOL_MAX ) * 0.10;
    o -> type = WARLOCK;

    for( player_t* p = sim -> player_list; p; p = p -> next )
    {
      if( p -> sleeping ) continue;

      p -> buffs.demonic_pact -> trigger( 1, buff );

      // HACK ALERT!!! Remove "double-dip" during spell power scale factor generation.
      if ( p != o && sim -> scaling -> scale_stat == STAT_SPELL_POWER )
      {
        p -> buffs.demonic_pact -> current_value -= sim -> scaling -> scale_value * 0.10;
      }
	  // HACK ALERT!!! Remove "double-dip" during spirit scale factor generation.
      if ( p != o && sim -> scaling -> scale_stat == STAT_SPIRIT )
      {
        p -> buffs.demonic_pact -> current_value -= sim -> scaling -> scale_value * o -> spell_power_per_spirit * 0.10;
		if ( o -> buffs_life_tap_glyph -> up() ) p -> buffs.demonic_pact -> current_value -= sim -> scaling -> scale_value * 0.20 * 0.10;
      }
    }
  }
};

// warlock_pet_t::register_callbacks ========================================

void warlock_pet_t::register_callbacks()
{
  warlock_t* o = owner -> cast_warlock();

  pet_t::register_callbacks();

  if ( o -> talent_demonic_pact -> rank )
  {
    action_callback_t* cb = new demonic_pact_callback_t( this );

    register_attack_result_callback( RESULT_CRIT_MASK, cb );
    register_spell_result_callback ( RESULT_CRIT_MASK, cb );
  }
}

// imp_pet_t::fire_bolt_t::execute ==========================================

void imp_pet_t::fire_bolt_t::execute()
{
  warlock_pet_spell_t::execute();
  warlock_t* p = player -> cast_pet() -> owner -> cast_warlock();
  p -> buffs_empowered_imp -> trigger();
  p -> procs_empowered_imp -> occur();
}

// ==========================================================================
// Warlock Character Definition
// ==========================================================================

// warlock_t::composite_spell_power =========================================

double warlock_t::composite_spell_power( int school ) SC_CONST
{
  double sp = player_t::composite_spell_power( school );

  sp += buffs_fel_armor -> value();

  if ( buffs_life_tap_glyph -> up() )
  {
    sp += spirit() * 0.20;
  }


  return sp;
}

double warlock_t::composite_spell_haste() SC_CONST
{
	double h = player_t::composite_spell_haste();
	 if ( buffs_dark_intent_feedback -> up() ) h /= ( 1.0 + 0.03 );

	 return h;
}

// warlock_t::create_action =================================================

action_t* warlock_t::create_action( const std::string& name,
                                    const std::string& options_str )
{
  if ( name == "chaos_bolt"          ) return new          chaos_bolt_t( this, options_str );
  if ( name == "conflagrate"         ) return new         conflagrate_t( this, options_str );
  if ( name == "corruption"          ) return new          corruption_t( this, options_str );
  if ( name == "bane_of_agony"       ) return new       bane_of_agony_t( this, options_str );
  if ( name == "bane_of_doom"        ) return new        bane_of_doom_t( this, options_str );
  if ( name == "curse_of_elements"   ) return new   curse_of_elements_t( this, options_str );
  if ( name == "death_coil"          ) return new          death_coil_t( this, options_str );
  if ( name == "demonic_empowerment" ) return new demonic_empowerment_t( this, options_str );
  if ( name == "drain_life"          ) return new          drain_life_t( this, options_str );
  if ( name == "drain_soul"          ) return new          drain_soul_t( this, options_str );
  if ( name == "fel_armor"           ) return new           fel_armor_t( this, options_str );
  if ( name == "fire_stone"          ) return new          fire_stone_t( this, options_str );
  if ( name == "spell_stone"         ) return new         spell_stone_t( this, options_str );
  if ( name == "haunt"               ) return new               haunt_t( this, options_str );
  if ( name == "immolate"            ) return new            immolate_t( this, options_str );
  if ( name == "immolation"          ) return new          immolation_t( this, options_str );
  if ( name == "shadowflame"         ) return new         shadowflame_t( this, options_str );
  if ( name == "incinerate"          ) return new          incinerate_t( this, options_str );
  if ( name == "life_tap"            ) return new            life_tap_t( this, options_str );
  if ( name == "metamorphosis"       ) return new       metamorphosis_t( this, options_str );
  if ( name == "shadow_bolt"         ) return new         shadow_bolt_t( this, options_str );
  if ( name == "shadowburn"          ) return new          shadowburn_t( this, options_str );
  if ( name == "shadowfury"          ) return new          shadowfury_t( this, options_str );
  if ( name == "searing_pain"        ) return new        searing_pain_t( this, options_str );
  if ( name == "soul_fire"           ) return new           soul_fire_t( this, options_str );
  if ( name == "summon_pet"          ) return new          summon_pet_t( this, options_str );
  if ( name == "unstable_affliction" ) return new unstable_affliction_t( this, options_str );
  if ( name == "hand_of_guldan" 	 ) return new      hand_of_guldan_t( this, options_str );
  if ( name == "fel_flame" 	 		 ) return new           fel_flame_t( this, options_str );
  if ( name == "dark_intent"         ) return new         dark_intent_t( this, options_str );




  return player_t::create_action( name, options_str );
}

// warlock_t::create_pet =====================================================

pet_t* warlock_t::create_pet( const std::string& pet_name )
{
  pet_t* p = find_pet( pet_name );

  if ( p ) return p;

  if ( pet_name == "felguard"  ) return new  felguard_pet_t( sim, this );
  if ( pet_name == "felhunter" ) return new felhunter_pet_t( sim, this );
  if ( pet_name == "imp"       ) return new       imp_pet_t( sim, this );
  if ( pet_name == "succubus"  ) return new  succubus_pet_t( sim, this );
  if ( pet_name == "infernal"  ) return new  infernal_pet_t( sim, this );
  if ( pet_name == "doomguard" ) return new doomguard_pet_t( sim, this );

  return 0;
}

// warlock_t::create_pets ====================================================

void warlock_t::create_pets()
{
  create_pet( "felguard"  );
  create_pet( "felhunter" );
  create_pet( "imp"       );
  create_pet( "succubus"  );
  create_pet( "infernal"  );
  create_pet( "doomguard" );
}

// warlock_t::init_glyphs =====================================================

void warlock_t::init_glyphs()
{
  memset( ( void* ) &glyphs, 0x0, sizeof( glyphs_t ) );

  std::vector<std::string> glyph_names;
  int num_glyphs = util_t::string_split( glyph_names, glyphs_str, ",/" );

  for ( int i=0; i < num_glyphs; i++ )
  {
    std::string& n = glyph_names[ i ];

    if     ( n == "chaos_bolt"          ) glyphs.chaos_bolt = 1;
    else if ( n == "conflagrate"         ) glyphs.conflagrate = 1;
    else if ( n == "corruption"          ) glyphs.corruption = 1;
    else if ( n == "bane_of_agony"       ) glyphs.bane_of_agony = 1;
    else if ( n == "felguard"            ) glyphs.felguard = 1;
    else if ( n == "felhunter"           ) glyphs.felhunter = 1;
    else if ( n == "haunt"               ) glyphs.haunt = 1;
    else if ( n == "immolate"            ) glyphs.immolate = 1;
    else if ( n == "imp"                 ) glyphs.imp = 1;
    else if ( n == "incinerate"          ) glyphs.incinerate = 1;
    else if ( n == "life_tap"            ) glyphs.life_tap = 1;
    else if ( n == "metamorphosis"       ) glyphs.metamorphosis = 1;
    else if ( n == "searing_pain"        ) glyphs.searing_pain = 1;
    else if ( n == "shadow_bolt"         ) glyphs.shadow_bolt = 1;
    else if ( n == "shadow_burn"         ) glyphs.shadow_burn = 1;
    else if ( n == "siphon_life"         ) glyphs.siphon_life = 1;
    else if ( n == "unstable_affliction" ) glyphs.unstable_affliction = 1;
    else if ( n == "quick_decay"         ) glyphs.quick_decay = 1;
    // minor glyphs, to prevent 'not-found' warning
    else if ( n == "curse_of_exhaustion" ) ;
    else if ( n == "curse_of_exhausion" )  ; // It's mis-spelt on the armory.
    else if ( n == "drain_soul" )          ;
    else if ( n == "enslave_demon" )       ;
    else if ( n == "healthstone" )         ;
    else if ( n == "howl_of_terror")       ;
    else if ( n == "kilrogg" )             ;
    else if ( n == "shadowflame" )         ;
    else if ( n == "soul_link" )           ;
    else if ( n == "souls" )               ;
    else if ( n == "soulstone" )           ;
    else if ( n == "unending_breath" )     ;
    else if ( n == "voidwalker" )          ;
    else if ( ! sim -> parent ) 
    {
      sim -> errorf( "Player %s has unrecognized glyph %s\n", name(), n.c_str() );
    }
  }
}

// warlock_t::init_race ======================================================

void warlock_t::init_race()
{
  race = util_t::parse_race_type( race_str );
  switch ( race )
  {
  case RACE_HUMAN:
  case RACE_GNOME:
  case RACE_UNDEAD:
  case RACE_ORC:
  case RACE_BLOOD_ELF:
  case RACE_WORGEN:
  case RACE_GOBLIN:
    break;
  default:
    race = RACE_UNDEAD;
    race_str = util_t::race_type_string( race );
  }

  player_t::init_race();
}

// warlock_t::init_base ======================================================

void warlock_t::init_base()
{
  player_t::init_base();

  attribute_multiplier_initial[ ATTR_STAMINA ] *= 1.0 + util_t::talent_rank( talent_demonic_embrace -> rank, 3, 0.04, 0.07, 0.10 );

  base_attack_power = -10;
  initial_attack_power_per_strength = 1.0;
  initial_spell_power_per_intellect = 1.0;

  health_per_stamina = 10;
  mana_per_intellect = 15;

}

// warlock_t::init_scaling ===================================================

void warlock_t::init_scaling()
{
  player_t::init_scaling();

}

// warlock_t::init_buffs =====================================================

void warlock_t::init_buffs()
{
  player_t::init_buffs();

  buffs_backdraft           = new buff_t( this, "backdraft",           3, 15.0, 0.0, talent_backdraft -> rank );
  buffs_decimation          = new buff_t( this, "decimation",          1,  8.0, 0.0, talent_decimation -> rank );
  buffs_demonic_empowerment = new buff_t( this, "demonic_empowerment", 1 );
  buffs_demonic_frenzy      = new buff_t( this, "demonic_frenzy",     10, 10.0 );
  buffs_empowered_imp       = new buff_t( this, "empowered_imp",       1,  8.0, 0.0, talent_empowered_imp -> rank * 0.02 );
  buffs_eradication         = new buff_t( this, "eradication",         1, 10.0, 0.0, talent_eradication -> rank ? 0.06 : 0.00 );
  buffs_fel_armor           = new buff_t( this, "fel_armor"     );
  buffs_haunted             = new buff_t( this, "haunted",             1, 12.0, 0.0, talent_haunt -> rank );
  buffs_life_tap_glyph      = new buff_t( this, "life_tap_glyph",      1, 40.0, 0.0, glyphs.life_tap );
  buffs_metamorphosis       = new buff_t( this, "metamorphosis",       1, 30.0 + glyphs.metamorphosis * 6.0, 0.0, talent_metamorphosis -> rank );
  buffs_molten_core         = new buff_t( this, "molten_core",         3, 15.0, 0.0, talent_molten_core -> rank * 0.04 );
  buffs_shadow_embrace      = new buff_t( this, "shadow_embrace",      3, 12.0, 0.0, talent_shadow_embrace -> rank );
  buffs_shadow_trance       = new buff_t( this, "shadow_trance",       1,  0.0, 0.0, talent_nightfall -> rank );
  buffs_tier10_4pc_caster   = new buff_t( this, "tier10_4pc_caster",   1, 10.0, 0.0, 0.15 ); // Fix-Me: Might need to add an ICD.
  buffs_hand_of_guldan		= new buff_t( this, "hand_of_guldan",	   1, 15.0, 0.0, talent_hand_of_guldan -> rank );
  buffs_improved_soul_fire	= new buff_t( this, "improved_soul_fire",  1, 15.0, 0.0, (talent_improved_soul_fire -> rank > 0) );
  buffs_dark_intent_feedback= new buff_t( this, "dark_intent_feedback", 1, 7.0 );

  buffs_tier7_2pc_caster = new      buff_t( this, "tier7_2pc_caster",                   1, 10.0, 0.0, set_bonus.tier7_2pc_caster() * 0.15 );
  buffs_tier7_4pc_caster = new stat_buff_t( this, "tier7_4pc_caster", STAT_SPIRIT, 300, 1, 10.0, 0.0, set_bonus.tier7_4pc_caster() );
}

// warlock_t::init_gains =====================================================

void warlock_t::init_gains()
{
  player_t::init_gains();

  gains_fel_armor  = get_gain( "fel_armor"  );
  gains_felhunter  = get_gain( "felhunter"  );
  gains_life_tap   = get_gain( "life_tap"   );
  gains_soul_leech = get_gain( "soul_leech" );
}

// warlock_t::init_uptimes ====================================================

void warlock_t::init_uptimes()
{
  player_t::init_uptimes();

}
// warlock_t::init_procs =====================================================

void warlock_t::init_procs()
{
  player_t::init_procs();

  procs_empowered_imp  = get_proc( "empowered_imp"  );
}

// warlock_t::init_rng =======================================================

void warlock_t::init_rng()
{
  player_t::init_rng();

  rng_soul_leech             = get_rng( "soul_leech"             );
  rng_everlasting_affliction = get_rng( "everlasting_affliction" );
  rng_pandemic				 = get_rng( "pandemic" 				 );
  rng_cremation				 = get_rng( "cremation"				 );
}

// warlock_t::init_talents ====================================================
void warlock_t::init_talents()
{
  player_t::init_talents();

  talent_spec_spells.unstable_affliction -> init_enabled();
  talent_spec_spells.shadow_mastery -> init_enabled();
  talent_spec_spells.summon_felguard -> init_enabled();
  talent_spec_spells.conflagrate -> init_enabled();
  talent_spec_spells.cataclysm -> init_enabled();

  mastery_spells.potent_afflictions -> init_enabled();
  mastery_spells.master_demonologist -> init_enabled();
  mastery_spells.fiery_apocalypse -> init_enabled();
}

// warlock_t::init_spells
void warlock_t::init_spells()
{
  player_t::init_spells();


}
// warlock_t::init_actions ===================================================

void warlock_t::init_actions()
{
  if ( action_list_str.empty() )
  {
    std::string tap_str = "life_tap";
    action_list_str += "flask,type=frost_wyrm/food,type=fish_feast";
    action_list_str += !( primary_tree() == TREE_DESTRUCTION ) ? "/spell_stone" : "/fire_stone";
    action_list_str += "/fel_armor/summon_pet";

    // Choose Pet
    if ( summon_pet_str.empty() )
    {
      if ( primary_tree() == TREE_DEMONOLOGY )
        summon_pet_str = "felguard";
      else if ( primary_tree() == TREE_DESTRUCTION )
        summon_pet_str = "imp";
      else if ( primary_tree() == TREE_AFFLICTION )
        summon_pet_str = "felhunter";
      else
        summon_pet_str = "succubus";
    }
    action_list_str += "," + summon_pet_str;
    action_list_str += "/snapshot_stats";

    // Refresh Life Tap Buff
    if( glyphs.life_tap )
    {
      action_list_str+="/" + tap_str + ",buff_refresh=1";
    }

    // Usable Item
    int num_items = ( int ) items.size();
    for ( int i=0; i < num_items; i++ )
    {
      if ( items[ i ].use.active() )
      {
        action_list_str += "/use_item,name=";
        action_list_str += items[ i ].name();
      }
    }

    // Race Skills
    if ( race == RACE_ORC )
    {
      action_list_str += "/blood_fury";
    }
    else if ( race == RACE_BLOOD_ELF )
    {
      action_list_str += "/arcane_torrent";
    }

    // Choose Potion
    if ( talent_haunt -> rank )
    {
    	action_list_str += "/wild_magic_potion,if=!in_combat";
    	action_list_str += "/speed_potion,if=buff.bloodlust.react";
    }
    else
    {
    	action_list_str += "/wild_magic_potion,if=(buff.bloodlust.react)|(!in_combat)";
    }


    switch ( primary_tree() )
    {
    case TREE_AFFLICTION:

      if ( talent_haunt -> rank ) action_list_str += "/haunt,if=(buff.haunted.remains<3)|(dot.corruption.remains<4)";
      action_list_str += "/corruption,if=!ticking";
      if ( talent_spec_spells.unstable_affliction -> enabled ) action_list_str += "/unstable_affliction,time_to_die>=5,if=(dot.unstable_affliction.remains<cast_time)";
      action_list_str += "/bane_of_agony,time_to_die>=20,if=!ticking";
      if ( talent_soul_siphon -> rank ) action_list_str += "/drain_soul,health_percentage<=25,interrupt=1";
      if ( talent_spec_spells.unstable_affliction -> enabled ) action_list_str +="/fel_flame,if=dot.unstable_affliction.remains<=4";
      action_list_str += "/drain_life";

    case TREE_DESTRUCTION:
      if ( talent_spec_spells.conflagrate -> enabled ) action_list_str += "/conflagrate";
      action_list_str += "/immolate,time_to_die>=3,if=(dot.immolate.remains<cast_time)";
      action_list_str += "/chaos_bolt";
      action_list_str += "/bane_of_doom,time_to_die>=70";
      action_list_str += "/bane_of_agony,moving=1,if=(!ticking)&!(dot.bane_of_doom.remains>0)";
      action_list_str += "/fel_flame,if=dot.immolate.remains<=4";
      action_list_str += "/incinerate";

    case TREE_DEMONOLOGY:
      if ( talent_demonic_empowerment -> rank ) action_list_str += "/demonic_empowerment";
      action_list_str += "/metamorphosis";
      action_list_str += "/immolate,time_to_die>=4,if=(dot.immolate.remains<cast_time)";
      action_list_str += "/immolation,if=(buff.tier10_4pc_caster.react)|(buff.metamorphosis.remains<15)";
      action_list_str += "/bane_of_doom,time_to_die>=70";
      if ( talent_decimation -> rank ) action_list_str += "/soul_fire,if=(buff.decimation.react)&(buff.molten_core.react)";
      action_list_str += "/corruption,time_to_die>=8,if=!ticking";
      if ( talent_decimation -> rank ) action_list_str += "/soul_fire,if=buff.decimation.react";
      action_list_str += "/incinerate,if=buff.molten_core.react";
		  // Set Mana Buffer pre 35% with or without Glyph of Life Tap
		  if( set_bonus.tier7_4pc_caster() || glyphs.life_tap )
			  {
			  action_list_str += "/life_tap,trigger=12000,health_percentage>=35,if=buff.metamorphosis.down";
			  }
		  else
			  {
			  action_list_str += "/life_tap,trigger=19000,health_percentage>=35,if=buff.metamorphosis.down";
			  }
	  action_list_str += "/bane_of_agony,moving=1,if=(!ticking)&!(dot.bane_of_doom.remains>0)";
	  action_list_str += "/fel_flame,if=dot.immolate.remains<=4";
	  action_list_str += "/shadow_bolt";

	default:

      action_list_str += "/corruption,if=!ticking/bane_of_agony,time_to_die>20,if=!ticking/immolate,if=(dot.immolate.remains<cast_time)";
      if ( sim->debug ) log_t::output( sim, "Using generic action string for %s.", name() );
    break;
	}

    // instants to use when moving if possible
    if ( talent_shadowburn -> rank ) action_list_str += "/shadowburn,moving=1";
    if ( talent_shadowfury -> rank  ) action_list_str += "/shadowfury,moving=1";

    action_list_str += "/" + tap_str; // to use when no mana or nothing else is possible

    action_list_default = 1;
  }

  player_t::init_actions();
}


// warlock_t::reset ==========================================================

void warlock_t::reset()
{
  player_t::reset();

  // Active
  active_pet                 = 0;


}

// warlock_t::regen ==========================================================

void warlock_t::regen( double periodicity )
{
  player_t::regen( periodicity );
}

// warlock_t::create_expression =================================================

action_expr_t* warlock_t::create_expression( action_t* a, const std::string& name_str )
{


  return player_t::create_expression( a, name_str );
}



// warlock_t::get_talent_trees =============================================

std::vector<talent_translation_t>& warlock_t::get_talent_list()
{
	  talent_list.clear();
	  return talent_list;
}

// warlock_t::get_options ==================================================

std::vector<option_t>& warlock_t::get_options()
{
  if ( options.empty() )
  {
    player_t::get_options();

    option_t warlock_options[] =
    {
      // @option_doc loc=player/warlock/talents title="Talents"
      { "backdraft",                OPT_TALENT_RANK, talent_backdraft                 },
      { "bane",                     OPT_TALENT_RANK, talent_bane                      },
      { "chaos_bolt",               OPT_TALENT_RANK, talent_chaos_bolt                },
      { "deaths_embrace",           OPT_TALENT_RANK, talent_deaths_embrace            },
      { "decimation",               OPT_TALENT_RANK, talent_decimation                },
      { "demonic_aegis",            OPT_TALENT_RANK, talent_demonic_aegis             },
      { "demonic_embrace",          OPT_TALENT_RANK, talent_demonic_embrace           },
      { "demonic_empowerment",      OPT_TALENT_RANK, talent_demonic_empowerment       },
      { "demonic_pact",             OPT_TALENT_RANK, talent_demonic_pact              },
      { "emberstorm",               OPT_TALENT_RANK, talent_emberstorm                },
      { "empowered_imp",            OPT_TALENT_RANK, talent_empowered_imp             },
      { "eradication",              OPT_TALENT_RANK, talent_eradication               },
      { "everlasting_affliction",   OPT_TALENT_RANK, talent_everlasting_affliction    },
      { "fire_and_brimstone",       OPT_TALENT_RANK, talent_fire_and_brimstone        },
      { "haunt",                    OPT_TALENT_RANK, talent_haunt                     },
      { "improved_corruption",      OPT_TALENT_RANK, talent_improved_corruption		  },
      { "improved_immolate",        OPT_TALENT_RANK, talent_improved_immolate         },
      { "improved_life_tap",        OPT_TALENT_RANK, talent_improved_life_tap         },
      { "improved_searing_pain",    OPT_TALENT_RANK, talent_improved_searing_pain     },
      { "shadow_and_flame",     	OPT_TALENT_RANK, talent_shadow_and_flame          },
      { "mana_feed",                OPT_TALENT_RANK, talent_mana_feed                 },
      { "metamorphosis",            OPT_TALENT_RANK, talent_metamorphosis             },
      { "molten_core",              OPT_TALENT_RANK, talent_molten_core               },
      { "nemesis",                  OPT_TALENT_RANK, talent_nemesis                   },
      { "nightfall",                OPT_TALENT_RANK, talent_nightfall                 },
      { "pandemic",                 OPT_TALENT_RANK, talent_pandemic                  },
      { "shadowburn",              	OPT_TALENT_RANK, talent_shadowburn                },
      { "siphon_life",              OPT_TALENT_RANK, talent_siphon_life               },
      { "soul_leech",               OPT_TALENT_RANK, talent_soul_leech                },
      { "soul_siphon",              OPT_TALENT_RANK, talent_soul_siphon               },

      // Spec Spells
      { "cataclysm",                OPT_SPELL_ENABLED, talent_spec_spells.cataclysm              },
      { "shadow_mastery",           OPT_SPELL_ENABLED, talent_spec_spells.shadow_mastery         },
      { "conflagrate",              OPT_SPELL_ENABLED, talent_spec_spells.conflagrate            },
      { "summon_felguard",          OPT_SPELL_ENABLED, talent_spec_spells.summon_felguard        },
      { "unstable_affliction",      OPT_SPELL_ENABLED, talent_spec_spells.unstable_affliction    },

      // Mastery Spells

      // @option_doc loc=player/warlock/misc title="Misc"
      { "summon_pet",               OPT_STRING, &( summon_pet_str                 		 ) },
      { "dark_intent_target",       OPT_STRING, &( dark_intent_target_str           	 ) },
      { NULL, OPT_UNKNOWN, NULL }
    };

    option_t::copy( options, warlock_options );
  }

  return options;
}

// warlock_t::decode_set ===================================================

int warlock_t::decode_set( item_t& item )
{
  if ( item.slot != SLOT_HEAD      &&
       item.slot != SLOT_SHOULDERS &&
       item.slot != SLOT_CHEST     &&
       item.slot != SLOT_HANDS     &&
       item.slot != SLOT_LEGS      )
  {
    return SET_NONE;
  }

  const char* s = item.name();

  if ( strstr( s, "plagueheart"  ) ) return SET_T7_CASTER;
  if ( strstr( s, "deathbringer" ) ) return SET_T8_CASTER;
  if ( strstr( s, "kelthuzads"   ) ) return SET_T9_CASTER;
  if ( strstr( s, "guldans"      ) ) return SET_T9_CASTER;
  if ( strstr( s, "dark_coven"   ) ) return SET_T10_CASTER;

  return SET_NONE;
}

// ==========================================================================
// PLAYER_T EXTENSIONS
// ==========================================================================

// player_t::create_warlock ================================================

player_t* player_t::create_warlock( sim_t* sim, const std::string& name, race_type r )
{
  return new warlock_t( sim, name, r );
}

// player_t::primary_tree ==================================================

talent_tree_type warlock_t::primary_tree() SC_CONST
{
	  if ( level > 10 && level <= 69 )
	  {
		  if ( talent_tab_points[ WARLOCK_AFFLICTION ] > 0 ) return TREE_AFFLICTION;
		  if ( talent_tab_points[ WARLOCK_DEMONOLOGY ] > 0 ) return TREE_DEMONOLOGY;
		  if ( talent_tab_points[ WARLOCK_DESTRUCTION ] > 0 ) return TREE_DESTRUCTION;
		  return TREE_NONE;
	  }
	  else
	  {
		  if ( talent_tab_points[ WARLOCK_DESTRUCTION ] >= talent_tab_points[ WARLOCK_AFFLICTION ] )
		  {
			  if ( talent_tab_points[ WARLOCK_DESTRUCTION ] >= talent_tab_points[ WARLOCK_DEMONOLOGY ] )
			  {
				  return TREE_DESTRUCTION;
			  }
			  else
			  {
				  return TREE_DEMONOLOGY;
			  }
		  }
		  else if ( talent_tab_points[ WARLOCK_DEMONOLOGY ] >= talent_tab_points[ WARLOCK_AFFLICTION ] )
		  {
			  return TREE_DEMONOLOGY;
		  }
		  else
		  {
			  return TREE_AFFLICTION;
		  }
	  }

}

// player_t::warlock_init ===================================================

void player_t::warlock_init( sim_t* sim )
{


  sim -> auras.demonic_pact         = new aura_t( sim, "demonic_pact", 1 );

  for( player_t* p = sim -> player_list; p; p = p -> next )
  {
    p -> buffs.demonic_pact = new buff_t( p, "demonic_pact", 1, 45.0 );
    p -> buffs.dark_intent  = new buff_t( p, "dark_intent", 1 );
  }

  target_t* t = sim -> target;
  t -> debuffs.improved_shadow_bolt = new     debuff_t( sim, "improved_shadow_bolt", 1, 30.0 );
  t -> debuffs.curse_of_elements    = new coe_debuff_t( sim );
}

// player_t::warlock_combat_begin ===========================================

void player_t::warlock_combat_begin( sim_t* sim )
{
  for ( player_t* p = sim -> player_list; p; p = p -> next )
  {
	  if ( p -> ooc_buffs() )
	  {
		  if ( sim -> overrides.dark_intent       ) p -> buffs.dark_intent       -> override();
	  }
  }

  if ( sim -> overrides.demonic_pact ) sim -> auras.demonic_pact -> override();

  target_t* t = sim -> target;
  if ( sim -> overrides.improved_shadow_bolt ) t -> debuffs.improved_shadow_bolt -> override();
  if ( sim -> overrides.curse_of_elements    ) t -> debuffs.curse_of_elements    -> override( 1, 13 );
}

