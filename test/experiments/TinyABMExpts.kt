package experiments

import TinyABM
import org.junit.Test

class TinyABMExpts {

    @Test
    fun fermionic1Timestep() {
        val fermionic = ABMConstraints.fermionicConstraints(1, TinyABM)
        println(fermionic)
    }
}