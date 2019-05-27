package com.shiftpsh.tetris_ai

import java.io.File
import java.nio.charset.Charset
import kotlin.random.Random



fun execute(constraints: Constraints): Result {
    val resultRaw = TetrisJNI.simulate(constraints.toString().split(' ').toTypedArray())
    return resultRaw
        .split('\n')[0]
        .split(' ')
        .map { it.toDouble() }
        .let { Result(it[0], it[1], it[2]) }
}

fun batch_execute(iterations: Int, constraints: Constraints): Result {
    val results = List(iterations) { execute(constraints) }.sortedBy { it.cost() }
    val truncated = results.subList(iterations / 8, 3 * iterations / 8)

    var average = Result(0.0, 0.0, 0.0)
    for (r in truncated) average += r
    return average / truncated.size.toDouble()
}

fun main() {
    var constraints = Constraints(List(18){0.0})

    val k = 0.005

    var temperature = 1000.0
    println("Epoch 0: temperature = $temperature")
    println()

    var prevResult = batch_execute(16, constraints)

    val startTime = System.currentTimeMillis()
    while (temperature > 0.001) {
        val newConstraints = constraints.gaussianNeighbor(50.0)
        println("Epoch $i: temperature = $temperature")
        val currResult = batch_execute(16, newConstraints)

        val p = Math.pow(Math.E, (prevResult.cost() - currResult.cost()) / (temperature * k))

        if (p > Random.nextDouble() || prevResult.cost() > currResult.cost()) {
            constraints = newConstraints
            prevResult = currResult
            temperature *= 0.995
            println("[v | %.3f%%] $currResult".format(Math.min(p * 100, 100.0)))
        } else {
            println("[x | %.3f%%] $currResult".format(Math.min(p * 100, 100.0)))
        }

        val endTime = System.currentTimeMillis()

        val s = ((endTime - startTime) / 1000).toDouble()
        val m = (s / 60).toLong()
        val h = (m / 60)
        println("Time elapsed: %02d h %02d m %.2f s".format(h, m % 60, s % 60.0))
        println()
    }
}