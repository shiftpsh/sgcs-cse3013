package com.shiftpsh.tetris_ai

import java.util.*

data class Constraints(
    val constraints: List<Double>
) {
    override fun toString(): String {
        return constraints.map { "%.2f".format(it) }.joinToString(" ")
    }

    operator fun get(i: Int) = constraints[i]

    operator fun unaryMinus() = Constraints(constraints.map { -it })

    operator fun plus(rhs: Constraints) = constraints.forEachIndexed { index, d -> d + rhs[index] }

    operator fun minus(rhs: Constraints) = this + (-rhs)

    operator fun times(rhs: Double) = Constraints(constraints.map { it * rhs })

    operator fun div(rhs: Double) = this * (1 / rhs)

    fun gaussian(mean: Double, variance: Double): Double {
        return mean + (Random().nextGaussian() - 0.5) * variance
    }

    fun gaussianNeighbor(variance: Double): Constraints = Constraints(constraints.map { gaussian(it, variance) })
}

operator fun Double.times(rhs: Constraints): Constraints {
    return rhs * this
}