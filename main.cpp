/******************************************************
 * This is the main file for the mips1 ArchC model    *
 * This file is automatically generated by ArchC      *
 * WITHOUT WARRANTY OF ANY KIND, either express       *
 * or implied.                                        *
 * For more information on ArchC, please visit:       *
 * http://www.archc.org                               *
 *                                                    *
 * The ArchC Team                                     *
 * Computer Systems Laboratory (LSC)                  *
 * IC-UNICAMP                                         *
 * http://www.lsc.ic.unicamp.br                       *
 ******************************************************/
 


const char *project_name="mips";
const char *project_file="mips1.ac";
const char *archc_version="2.0beta1";
const char *archc_options="-abi -dy ";

#include <systemc.h>
#include "mips/mips.H"
#include "mmu/mmu.h"
#include "memory/memory.h"
#include "bus/bus.h"
#include "irqmp/irqmp.h"
#include "gptimer/gptimer.h"

using grlib::irqmp;
using grlib::gptimer;

int sc_main(int ac, char *av[])
{
  // Clock
  sc_clock p_clock("p_clock", 4, SC_NS);

  //!  ISA simulator
  cout << "Creating Processor" << endl;
  mips mips_proc1("mips");
  //! MMU
  cout << "Creating mmu" << endl;
  ac_tlm_mmu mmu("mmu");
  //! Bus
  cout << "Creating Bus routing system" << endl;
  ac_tlm_bus bus("bus");
  //! Memory
  cout << "Creating Memory" << endl;
  ac_tlm_mem mem("mem");
  //! Interrupt Control Unit
  cout << "Creating Interrupt Controller" << endl;
  irqmp irq("irq");
  //! GPtimer Frequency 40Hz
  cout << "Creating Timer" << endl;
  gptimer timer("timer",40);

#ifdef AC_DEBUG
  ac_trace("mips1_proc1.trace");
#endif 

  // CPU interrupt port
  irq.CPU_port[0](mips_proc1.intp);

  // Clock connections with Peripherals
  timer.clk(p_clock);
  irq.clk(p_clock);

  // Peripherals connected to Interrupt Controller by ports
  timer.IRQ_port(irq.target_export);

  mips_proc1.DM(mmu.target_export);
  mips_proc1.ack_port(irq.target_export);
  mmu.BUS_port(bus.target_export);
  bus.MEM_port(mem.target_export);
  bus.IRQ_port(irq.target_export);
  bus.GPTIMER_port(timer.target_export);

  mips_proc1.init(ac, av);
  mips_proc1.set_prog_args();
  cerr << endl;

  sc_start();

  mips_proc1.PrintStat();
  cerr << endl;

#ifdef AC_STATS
  mips1_proc1.ac_sim_stats.time = sc_simulation_time();
  mips1_proc1.ac_sim_stats.print();
#endif 

#ifdef AC_DEBUG
  ac_close_trace();
#endif 

  return mips_proc1.ac_exit_status;
}